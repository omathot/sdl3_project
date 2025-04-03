#pragma once

class Entity {
public:
  Entity();

  virtual bool move();
  virtual void render();

private:
  unsigned int _id;
};
