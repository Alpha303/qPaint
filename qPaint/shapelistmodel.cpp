#include "shapelistmodel.h"
#include "commands.h"
#include "paintarea.h"


ShapeListModel::ShapeListModel(PaintArea* parent):
    QAbstractListModel(parent),
    undoStack(*parent->getUndoStack())

{
}

Qt::ItemFlags ShapeListModel::flags(const QModelIndex& index) const
{
    if (index.isValid())
    {
        return  QAbstractListModel::flags(index)| Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    }

    return Qt::ItemIsSelectable|Qt::ItemIsDragEnabled| Qt::ItemIsDropEnabled|Qt::ItemIsEnabled;
}

Qt::DropActions ShapeListModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

const QList<BaseShape*>& ShapeListModel::getShapeListConst() const
{
    return shapeList;
}

QList<BaseShape*>& ShapeListModel::getShapeList()
{
    return shapeList;
}

int ShapeListModel::rowCount(const QModelIndex& parent) const
{
    parent;
    return shapeList.size();
}

QVariant ShapeListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return QString("Column %1").arg(section);
    else
        return QString("Row %1").arg(section);
}

QVariant ShapeListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    if (index.row() >= shapeList.size())
    {
        return QVariant();
    }
    if (role == Qt::DisplayRole)
    {
        return QVariant::fromValue(shapeList.at(index.row())->getName());
    }
    else
    {
        return QVariant();
    }
}

bool ShapeListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    /* Ovo je tvornička Qt funkcija čija svrha je unos podataka na određeno mjesto unutar
     * modela podataka. Najčešće se koristi pri uređivanju podataka na listi, primjerice,
     * kad korisnik napravi dvoklik na neki unos i promjeni njegovu vrijednost, ta promjena
     * vrši se kroz ovu funkciju.
     * U ovom slučaju:
     * -index - redak unutar liste nad kojim želimo izvršiti neku promjenu podataka
     * -value - QVariant kontejner koji sadrži podatke koji će biti unešeni u model
     * -role - uloga koja definira kontekst ubacivanja podataka */

    if (index.isValid() && role == Qt::UserRole)
    {
        /* Qt::UserRole je vlastito definirana uloga koja se u ovom slučaju koristi za
         * dodavanje novog oblika u određeni redak u modelu podataka. Prije toga nužno
         * je provjeriti je li indeks na koji se želi dodati oblik validan. */

        BaseShape* shape {nullptr}; //Prvo radimo novi prazni pokazivač osnovne klase.
        if (value.canConvert<BaseShape*>()) //Nužno je provjeriti da li je podatak kompatibilan s baznom klasom.
        {
            shape = value.value<BaseShape*>(); //Ukoliko je, pokazivaču dodjeljujemo adresu oblika spremljenu u value.
        }
        shapeList.replace(index.row(), shape); //Na prethodno ubačen prazan redak ubacujemo sadržaj (adresu novog oblika).
        emit dataChanged(index, index); //Odašiljemo signal kojim obavještavamo ostatak modela o promjeni sadržaja na tom indeksu.
        return true; //Vraćanjem true dajemo do znanja da je operacija uspješno obavljena.
    }

    if (index.isValid() && role == Qt::EditRole)
    {
        /* Qt::EditRole koristi se kada korisnik napravi dvoklik na naziv određenog oblika,
         * što mu omogućava da promjeni njegov naziv. */

        if (!value.toString().isEmpty() && !value.toString().trimmed().isEmpty())
        {
            shapeList[index.row()]->setName(value.toString()); //Obliku na položaju index dodjeljuje se upisano ime.
            emit dataChanged(index, index); //Odašilje se signal koji obavještava o promjeni podataka na tom indeksu.
            return true; //Vraća se poruka o uspješno obavljenoj operaciji.
        }
    }


    else if (role == Qt::DisplayRole)
    {
        /* Qt::DisplayRole aktivan je kada korisnik primjerice pokušava promjeniti redosljed oblika na listi (drag-n-drop). */

        BaseShape* shape {nullptr};
        for (int i = 0; i < shapeList.size(); i++)
        {
            if (i == index.row())
            {
                //do nothing
            }
            else
            {
                if (shapeList.at(i)->getName() == value.toString())
                {
                    shape = shapeList.at(i);
                    if (shape)
                    {
                        shapeList.replace(index.row(), shape);
                        emit dataChanged(index, index);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool ShapeListModel::insertRows(int row, int count, const QModelIndex& parent)
{   //Ubacuje prazan redak u listu oblika (koristi se samo prilikom
    // drag-n-drop operacija nad oblicima).
    beginInsertRows(QModelIndex(), row, row);
    shapeList.insert(row, nullptr);
    endInsertRows();
    return true;
}

bool ShapeListModel::removeRows(int row, int count, const QModelIndex& parent)
{
    //Briše redak iz liste (koristi se samo prilikom drag-n-drop
    //operacija nad oblicima).
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    shapeList.removeAt(row);
    endRemoveRows();
    return true;
}

void ShapeListModel::insertShapeAt(BaseShape* newShape, int row)
{
    insertRows(row, 1, QModelIndex()); //Pozivom funkcije insertRows dodaje se novi prazan redak na indeksu nula.
    QModelIndex index{QAbstractListModel::index(row, 0, QModelIndex())}; //Stvara se novi indeks koji označava taj redak.
    QVariant data; //Stvara se nova varijabla tipa QVariant koja će upakirati novi oblik.
    data.setValue(newShape); //Varijabli data se dodjeljuje adresa novog oblika.
    setData(index, data, Qt::UserRole); //Pozivom na funkciju setData, umetnutom praznom retku dodjeljujemo podatak (adresu novog oblika).
}

BaseShape* ShapeListModel::removeShapeAt(int row)
{
    BaseShape* shape{shapeList.at(row)};                                    //Uzmi prvi oblik iz liste i spremi ga u pokazivač shape
    removeRows(row, 1, QModelIndex());
    QModelIndex removedShapeIndex = index(row);                             //Definiraj indeks obrisanog retka
    emit dataChanged(removedShapeIndex, removedShapeIndex);                 //Signaliziraj promjenu podataka na tom indeksu
    return shape;
}

void ShapeListModel::swapList(QList<BaseShape*>& newList)
{
    if (newList.isEmpty() && shapeList.isEmpty())
    {
        //ništa
    }
    else
    {
        shapeList.swap(newList);
        emit dataChanged(index(0), index(newList.size() - 1));
    }
}

void ShapeListModel::deleteShapeViaContextMenu(int row)
{
    /* Ova funkcija briše oblik na specifičnom položaju
     * koji je definiran kada korisnik desnim klikom miša
     * otvori kontekstni meni nad nekim oblikom u listi. */

    undoStack.push(new DeleteShapeCommand(*this, row)); //Na undo hrpu se dodaje nova komanda brisanja koja se odnosi na redak koji je korisnik kliknuo.
}


void ShapeListModel::moveShapeUp(int row)
{
    if (0 < row && row < shapeList.size())
    {
        beginMoveRows(QModelIndex(), row, row, QModelIndex(), row - 1);
        shapeList.swap(row, row - 1);
        endMoveRows();
        emit dataChanged(QModelIndex(index(row)), QModelIndex(index(row - 1)));
    }
}

void ShapeListModel::moveShapeDown(int row)
{
    if (row + 1 < shapeList.size())
    {
        beginMoveRows(QModelIndex(), row, row, QModelIndex(), row + 2);
        shapeList.swap(row, row + 1);
        endMoveRows();
        emit dataChanged(QModelIndex(index(row)), QModelIndex(index(row + 1)));
    }
}
