#include "shape.hpp"
#include "rectangle.hpp"
#include "circle.hpp"

void testShape(jianing::Shape* shape)
{
  shape->getArea();
  shape->getFrameRect();
  shape->move({12, 7});
  shape->move(4, -9);
}

int main()
{
	jianing::Rectangle rectangle({5.0, 9.0, 13.0, 21.0});
  testShape(&rectangle);

  jianing::Circle circle(6.0, {3.0, 7.0});
  testShape(&circle);

  return 0;
}
