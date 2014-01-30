#pragma once

class Renderable {
public:
	virtual ~Renderable() {}
	virtual void render() = 0;
};

/*
class IDemo
{
public:
virtual ~IDemo() {}
virtual void OverrideMe() = 0;
};

class Parent
{
public:
virtual ~Parent();
};

class Child : public Parent, public IDemo
{
public:
virtual void OverrideMe()
{
//do stuff
}
};
*/