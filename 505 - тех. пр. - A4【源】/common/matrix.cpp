#include "matrix.hpp"
#include <iostream>
#include <algorithm>
#include <exception>
#include <memory>
#include "base-types.hpp"
#include "layer.hpp"
const size_t INITCAPACITY = 3;

jianing::Maxtrix::Maxtrix() :
  size_(0),
  capacity_(INITCAPACITY),
  layer_array_(allocator_layer_ptr.allocate(INITCAPACITY))
{
  for (size_t i = 0; i < INITCAPACITY; ++i)
  {
    allocator_layer_ptr.construct(&layer_array_[i], *LayerPtr());
    layer_array_[0]->allocator_shape_ptr.allocate(INITCAPACITY);
  }

}

jianing::Maxtrix::Maxtrix(const Maxtrix& copied_object) :
  size_(copied_object.size_),
  capacity_(copied_object.capacity_),
  layer_array_(allocator_layer_ptr.allocate(copied_object.capacity_))
{
  for (size_t i = 0; i < size_; ++i)
  {
    allocator_layer_ptr.construct(&layer_array_[i], copied_object[i]);
  }
}

jianing::Maxtrix::Maxtrix(Maxtrix&& moved_object) :
  size_(moved_object.size_),
  capacity_(moved_object.capacity_),
  layer_array_(moved_object.layer_array_)
{
  moved_object.size_ = 0;
  moved_object.capacity_ = 0;
  moved_object.layer_array_ = nullptr;
}

jianing::Maxtrix::~Maxtrix()
{
  for (size_t i = 0; i < size_; ++i)
  {
    allocator_layer_ptr.destroy(&layer_array_[i]);
  }
  allocator_layer_ptr.deallocate(layer_array_, capacity_);
}


jianing::Maxtrix& jianing::Maxtrix::operator=(const jianing::Maxtrix& copied_object)
{
  if (this != &copied_object)
  {
    for (size_t i = 0; i < size_; ++i)
    {
      allocator_layer_ptr.destroy(&layer_array_[i]);
    }
    allocator_layer_ptr.deallocate(layer_array_, capacity_);

    size_ = copied_object.size_;
    capacity_ = copied_object.capacity_;
    layer_array_ = allocator_layer_ptr.allocate(capacity_);

    for (size_t i = 0; i < size_; ++i)
    {
      allocator_layer_ptr.construct(&layer_array_[i], copied_object[i]);
    }
  }

  return *this;
}

jianing::Maxtrix& jianing::Maxtrix::operator=(jianing::Maxtrix&& moved_object)
{
  if (this != &moved_object)
  {
    for (size_t i = 0; i < size_; ++i)
    {
      allocator_layer_ptr.destroy(&layer_array_[i]);
    }
    allocator_layer_ptr.deallocate(layer_array_, capacity_);

    size_ = moved_object.size_;
    capacity_ = moved_object.capacity_;
    layer_array_ = moved_object.layer_array_;

    moved_object.size_ = 0;
    moved_object.capacity_ = 0;
    moved_object.layer_array_ = nullptr;
  }

  return *this;
}

jianing::Maxtrix::LayerPtr jianing::Maxtrix::operator[](const size_t index_layer) const
{
  if (index_layer >= size_)
  {
    throw std::out_of_range("Index can not be"
        + std::to_string(index_layer) + " ! Must smaller than"
        + std::to_string(size_) + " !\n");
  }

  return layer_array_[index_layer];
}

void jianing::Maxtrix::addShape(const jianing::Shape::ShapePtr& shape_new)
{
  if (shape_new == nullptr)
  {
    throw std::invalid_argument("Can not add shape which is null!\n");
  }

  for (size_t i = 0; i < size_; ++i)
  {
    if (isOverlap(layer_array_[i], shape_new) == false)
    {
      (layer_array_[i])->addShape(shape_new);

      return;
    }
  }

  addNewLayer();

  (layer_array_[size_ - 1])->addShape(shape_new);
}

// Determine whether the given shape intersects the shape in the current layer
bool jianing::Maxtrix::isOverlap(const LayerPtr& layer,const jianing::Shape::ShapePtr& shape) const
{
  if (layer == nullptr)
  {
    throw std::invalid_argument("The layer is null!\n");
  }

  if (shape == nullptr)
  {
    throw std::invalid_argument("The shape is null!\n");
  }

  double top_current = 0.0;
  double bottom_current = 0.0;
  double left_current = 0.0;
  double right_current = 0.0;
  const double top_judgment = shape->getFrameRect().pos.x + shape->getFrameRect().height / 2.0;
  const double bottom_judgment = shape->getFrameRect().pos.y - shape->getFrameRect().height / 2.0;
  const double left_judgment = shape->getFrameRect().pos.x - shape->getFrameRect().width / 2.0;
  const double right_judgment = shape->getFrameRect().pos.x + shape->getFrameRect().width / 2.0;
  rectangle_t current_layer_frame = {0.0, 0.0, 0.0, 0.0};
  bool overlap = false;

  for (size_t i = 0; i < size_; ++i)
  {
    try
    {
      current_layer_frame = layer->array_[i]->getFrameRect();
    }
    catch (const std::logic_error& error)
    {
      continue;
    }

    top_current = current_layer_frame.pos.y + current_layer_frame.height / 2.0;
    bottom_current = current_layer_frame.pos.y - current_layer_frame.height / 2.0;
    left_current = current_layer_frame.pos.x - current_layer_frame.width / 2.0;
    right_current = current_layer_frame.pos.x + current_layer_frame.width / 2.0;

    if (((top_judgment > top_current) || (bottom_judgment < bottom_current)) && ((left_judgment < left_current) || (right_judgment > right_current)))
    {
      continue;
    }
    else if (((left_judgment < left_current) || (right_judgment > right_current)) && ((top_judgment > top_current) || (bottom_judgment < bottom_current)))
    {
      continue;
    }
    else
    {
      overlap = true;
      break;
    }
  }

  return overlap;
}

size_t jianing::Maxtrix::getNumberLayer() const
{
  return size_;
}

void jianing::Maxtrix::addNewLayer()
{
  if (size_ == capacity_)
  {
    reserve(2 * size_);
  }

  ++size_;
}

void jianing::Maxtrix::reserve(const size_t new_capacity)
{
  LayerPtr* new_array = allocator_layer_ptr.allocate(new_capacity);

  for (size_t i = 0; i < size_; ++i)
  {
    allocator_layer_ptr.construct(&new_array[i], layer_array_[i]);
  }

  for (size_t i = 0; i < size_; ++i)
  {
    allocator_layer_ptr.destroy(&layer_array_[i]);
  }
  allocator_layer_ptr.deallocate(layer_array_, capacity_);

  layer_array_ = new_array;
  capacity_ = new_capacity;
}
