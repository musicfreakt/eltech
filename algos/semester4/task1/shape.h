#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include <list>
#include <string>
#include "screen.h"

char screen[YMAX][XMAX];
enum color { black = '*', white = '.' };

void screen_init()
{
    for (auto y = 0; y < YMAX; ++y)
        for (auto &x : screen[y])
            x = white;
}

void screen_destroy()
{
    for (auto y = 0; y < YMAX; ++y)
        for (auto &x : screen[y])
            x = black;
}

bool on_screen(int a, int b) // проверка попадания точки на экран
{
    return 0 <= a && a < XMAX && 0 <= b && b < YMAX;
}

void put_point(int a, int b)
{
    if (on_screen(a,b))
        screen[b][a] = black;
}

void put_line(int x0, int y0, int x1, int y1)
/* Алгоритм Брезенхэма для прямой:
рисование отрезка прямой от (x0,y0) до (x1,y1).
Уравнение прямой: b(x-x0) + a(y-y0) = 0.
Минимизируется величина abs(eps), где eps = 2*(b(x-x0)) + a(y-y0).
*/
{
    int dx = 1;
    int a = x1 - x0; if (a < 0) dx = -1, a = -a;
    int dy = 1;
    int b = y1 - y0; if (b < 0) dy = -1, b = -b;
    int two_a = 2*a;
    int two_b = 2*b;
    int xcrit = -b + two_a;
    int eps = 0;
    for (;;) { //Формирование прямой линии по точкам
        put_point(x0, y0);
        if (x0 == x1 && y0 == y1) break;
        if (eps <= xcrit) x0 += dx, eps += two_b;
        if (eps >= a || a < b) y0 += dy, eps -= two_a;
    }
}

void screen_clear(){ screen_init(); } //Очистка экрана

void screen_refresh() // Обновление экрана
{
    for (int y = YMAX-1; 0 <= y; --y) // с верхней строки до нижней
    {
        for (auto x : screen[y]) // от левого столбца до правого
            std::cout << x;
        std::cout << '\n';
    }
}

struct shape
{
    static list<shape> shapes; // Список фигур (один на все фигуры!)
    shape() {shapes.push_back(this);} //Фигура присоединяется к списку
    virtual point north() const = 0; //Точки для привязки
    virtual point south() const = 0;
    virtual point east() const = 0;
    virtual point west() const = 0;
    virtual point neast() const = 0;
    virtual point seast() const = 0;
    virtual point nwest() const = 0;
    virtual point swest() const = 0;
    virtual void draw() = 0; //Рисование
    virtual void move(int, int) = 0; //Перемещение
    virtual void resize(int) = 0; //Изменение размера
    private:
        shape(const shape&);
        shape(const shape&&);
};

list <shape> shape::shapes; // Размещение списка фигур
void shape_refresh() // Перерисовка всех фигур на экране
{
    screen_clear();
    for (auto p : shape::shapes)
        p->draw(); //Динамическое связывание!!!
    screen_refresh();
}


class rotatable : public virtual shape { //Фигуры, пригодные к повороту
    public:
        virtual void rotate_left() = 0;
        virtual void rotate_right() = 0;
};

class reflectable : public virtual shape {//Фигуры пригодные к зеркальному отражению
    public:
        virtual void flip_horisontally() = 0;
        virtual void flip_vertically() = 0;
};

// Линия
class line : public shape {
	/*
	Отрезок прямой ["w", "e"]
	north() определяет точку "выше центра отрезка и так далеко на север,
	как самая его северная точка", и т.п.
	*/
    line(const line&);
	line(const line&&);

    protected:
        point w, e;
    public:
        line(point a, point b) :w(a), e(b) {}
    	line(point a, int l) :w(a), e(point(a.x + l - 1, a.y)) {}

    	point north() const { return point((w.x + e.x) / 2, e.y < w.y ? w.y : e.y); }
    	point south() const { return point((w.x + e.x) / 2, e.y < w.y ? e.y : w.y); }
    	point east() const { return e; }
    	point west() const { return w; }
    	point neast() const { return e; }
    	point seast() const { return e; }
    	point nwest() const { return w; }
    	point swest() const { return w; }
    	void move(int a, int b) { w.x += a; w.y += b;e.x += a; e.y += b; }
    	void draw(){put_line(w, e);}
        void resize(int d){e.x += (e.x - w.x) * (d - 1); e.y += (e.y - w.y) * (d - 1);}
};

// Прямоугольник
class rectangle: public rotatable {
	/*
	nw-----n-----ne
	|             |
	|             |
	w      c      e
	|             |
	|             |
	sw-----s-----se
	*/
	rectangle(const rectangle&);
	rectangle(const rectangle&&);

    protected:
        point sw, ne;
    public:
    	point north() const { return point((sw.x + ne.x) / 2, ne.y); }
    	point south() const { return point((sw.x + ne.x) / 2, sw.y); }
    	point east() const { return point(ne.x, (sw.y + ne.y) / 2); }
    	point west() const { return point(sw.x, (sw.y + ne.y) / 2); }
    	point neast() const { return ne; }
    	point seast() const { return point(ne.x, sw.y); }
    	point nwest() const { return point(sw.x, ne.y); }
    	point swest() const { return sw; }

    	void rotate_right()//Поворот относительно se
    	{
    		int w = ne.x - sw.x, h = ne.y - sw.y;
    		sw.x = ne.x - h * 2;
    		ne.y = sw.y + w / 2;
    	}

    	void rotate_left() //Поворот относительно sw
    	{
    		int w = ne.x - sw.x, h = ne.y - sw.y;
    		ne.x = sw.x + h * 2;
    		ne.y = sw.y + w / 2;
    	}

    	void move(int a, int b)
    	{
    		sw.x += a;
    		sw.y += b;
    		ne.x += a;
    		ne.y += b;
    	}

        void resize(int d)
        {
            ne.x += (ne.x - sw.x) * (d - 1);
            ne.y += (ne.y - sw.y) * (d - 1);
        }
    	void draw();
    	rectangle(point, point);
};

rectangle::rectangle(point a, point b)
{
	if(a.x<=b.x)
		if (a.y <= b.y) {
			sw = a;
			ne = b;
		}
		else {
			sw = point(a.x, b.y);
			ne = point(b.x, a.y);
		}
	else
		if (a.y <= b.y) {
			sw = point(b.x, a.y);
			ne = point(a.x, b.y);
		}
		else {
			sw = b;
			ne = a;
		}
}
void rectangle::draw()
{
	point nw(sw.x, ne.y);
	point se(ne.x, sw.y);
	put_line(nw, ne);
	put_line(ne, se);
	put_line(se, sw);
	put_line(sw, nw);
}


// Трапеция

class trapeze : public rotatable, public reflectable
{

};


// Косой крест





void shape_refresh()//Перерисовка всех фигур
{
	screen_clear();
	for (shape* p = shape::list; p; p = p->next) p->draw();
	screen_refresh();
}

void up(shape* p, const shape* q)//Поместить p над q
{
	point n = q->north();
	point s = p->south();
	p->move(n.x - s.x, n.y - s.y + 1);
}

void lftUp(shape* p, const shape* q)//Поместить p слева над q
{
	p->move(q->nwest().x - p->swest().x, q->nwest().y - p->swest().y + 1);
}

void rgtUp(shape* p, const shape* q)//Поместить p справа над q
{
	p->move(q->neast().x - p->seast().x, q->nwest().y - p->swest().y + 1);
}

void rgtDwn(shape* p, const shape* q)//Поместить p справа под q
{
	p->move(q->east().x - p->west().x, q->swest().y - p->nwest().y);
}

void lftDwn(shape* p, const shape* q)//Поместить p справа под q
{
	p->move(q->west().x - p->east().x, q->swest().y - p->nwest().y);
}

#endif