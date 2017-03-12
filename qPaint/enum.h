#ifndef ENUM
#define ENUM

enum DrawMode {Type_Selection, Type_Freehand, Type_Line, Type_Rectangle, Type_Ellipse, Type_Polygon};
enum ShapeState {inProgress, finished, canceled};
enum MouseState {idle, targetting, dragging};

#endif // ENUM