/*
    Пополнение и использование библиотеки фигур
*/
#include "screen.h"
#include "shape.h"

using namespace std;

// ========== ДОБАВКА ============

// Трапеция

class trapezium : public rotatable, public reflectable
/*
          nw-----n-----ne
         /              \
        /                \
       w                  e
      /                    \
     /                      \
    sw----------s-----------se
*/
{
    trapezium(const trapezium&);
    trapezium(const trapezium&&);

    protected:
        point sw, nw, ne, se;
    public:
        trapezium(point, int, point, int);

        point north() const { return point((sw.x + ne.x) / 2, ne.y); }
        point south() const { return point((sw.x + se.x) / 2, sw.y); }
        point east() const { return point((ne.x + se.x) / 2, (ne.y + se.y) / 2); }
        point west() const { return point((nw.x + sw.x) / 2, (nw.y + sw.y) / 2); }
        point neast() const { return ne; }
        point seast() const { return se; }
        point nwest() const { return nw; }
        point swest() const { return sw; }

        void rotate_left();
        void rotate_right();
        void flip_horisontally();
        void flip_vertically();
        void move(int, int);
        void resize(int);
        void draw();
};

trapezium :: trapezium (point a, int lena, point b, int lenb)
{
    sw = a;
    nw = b;
    ne.x = nw.x + lenb; ne.y = nw.y;
    se.x = sw.x + lena; se.y = sw.y;
}

void trapezium :: rotate_right()
{
    // swap();
    // swap();
}

void trapezium :: rotate_left()
{
    // swap();
    // swap();
}

void trapezium :: flip_horisontally()
{
    // swap();
    // swap();
}

void trapezium :: flip_vertically()
{
    // swap();
    // swap();
}

void trapezium :: move(int a, int b)
{
    sw.x += a; sw.y += b;
    nw.x += a; nw.y += b;
    ne.x += a; ne.y += b;
    se.x += a; se.y += b;
}

void trapezium :: resize(int d)
{
    nw.y += (nw.y - sw.y) * (d - 1);
    se.x += (se.x - sw.x) * (d - 1);
    ne.x += (ne.x - nw.x) * (d - 1);
    ne.y += (ne.y - se.y) * (d - 1);
}

void trapezium :: draw()
{
	put_line(nw, ne);
	put_line(ne, se);
	put_line(se, sw);
	put_line(sw, nw);
}

// Косой крест

class cross : public rectangle
/*
    nw     n     ne
      \        /
        \     /
    w      c      e
        /     \
      /         \
    sw     s     se
*/
{
    cross(const cross&);
    cross(const cross&&);

    public:
        cross(point a, point b) : rectangle (a,b) {}
        void draw();
};

void cross :: draw()
{
    put_line(swest(), neast());
    put_line(nwest(), seast());
}


// Трапеция с косым крестом

class crossed_trapezium : public trapezium, public cross
{
    crossed_trapezium(const cross&);
    crossed_trapezium(const cross&&);

    public:
        crossed_trapezium(point a, int lena, point b, int lenb):
        trapezium(a, lena, b, lenb), cross(a, point(a.x + lena, b.y)) {}

        point north() const{ return trapezium::north(); } // север
        point south() const{ return trapezium::south(); } // юг
        point east() const{ return trapezium::east(); } // восток
        point west() const{ return trapezium::west(); } // запад
        point neast() const{ return trapezium::neast(); } // северо-восток
        point seast() const{ return trapezium::seast(); } // юго-восток
        point nwest() const{ return trapezium::nwest(); } // северо-запад
        point swest() const{ return trapezium::swest(); } // северо-восток

        void move(int, int);
        void resize(int);
        void draw();
};

void crossed_trapezium::move(int a, int b)
{
    trapezium::move(a, b);
    cross::move(a, b);
}

void crossed_trapezium::resize(int d)
{
    trapezium::resize(d);
    cross::resize(d);
}

void crossed_trapezium::draw()
{
    trapezium::draw();
    cross::draw();
}

void down(shape& p, const shape& q)
// Поместить p над q
{
	point n = q.south();
	point s = p.north();
	p.move(n.x - s.x, n.y - s.y - 1);
}

void left_up(shape& p, const shape& q)
// Поместить p слева над q
{
	p.move(q.nwest().x - p.swest().x, q.nwest().y - p.swest().y + 1);
}

void right_up(shape& p, const shape& q)
// Поместить p справа над q
{
	p.move(q.neast().x - p.seast().x, q.nwest().y - p.swest().y + 1);
}

void right_down(shape& p, const shape& q)
// Поместить p справа под q
{
	p.move(q.east().x - p.west().x, q.swest().y - p.nwest().y);
}

void left_down(shape& p, const shape& q)
// Поместить p справа под q
{
	p.move(q.west().x - p.east().x, q.swest().y - p.nwest().y);
}



// ========== КОНЕЦ ДОБАВКИ ============

// class face: public rectangle
// {
//
// };


int main()
{
    screen_init();
    // объявление набора фигур
    trapezium cs(point(1, 1), 10, point(4, 4), 5);
    cs.resize(8);
    shape_refresh();
    // подготовка к сборке

    // сборка изображения

    screen_destroy();
    return 0;
}
