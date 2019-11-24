#include <iostream>
#include <cstdio>
#include <cassert>
#include <cmath>
#include <ctime>
using std::cin;
using std::cout;
using std::endl;

#include "super_define.h"
#include "dump.h"

#include "Diff_Tree.h"

int main() {
    srand(time(NULL));

    Diff_Tree Q;

    Q.get_from_file(X_("equation.txt"));

    Q.open_tex();

    Q.tex_section("Вы ввели:");

    Q.tex("Вот же ответ: ");
    Q.put_tex(Q.head_);

    Q.tex_section("Возьмём производную");

    Q.d(Q.head_);

    Q.tex("Вот же ответ: ");
    Q.put_tex(Q.head_);

    Q.tex_section("Упростим:");

    Q.conv_const(Q.head_);

    Q.tex("Вот же ответ: ");
    Q.put_tex(Q.head_);

    Q.tex_section("Возьмём 2 производную");

    Q.d(Q.head_);

    Q.tex("Вот же ответ: ");
    Q.put_tex(Q.head_);

    Q.tex_section("Упростим:");

    Q.conv_const(Q.head_);

    Q.tex("Вот же ответ: ");
    Q.put_tex(Q.head_);

    Q.tex_section("Значение при х = 1: ");

    Q.put_var('x', 1.);
    Q.conv_const(Q.head_);

    Q.tex("Вот же ответ: ");
    Q.put_tex(Q.head_);

    Q.close_tex();

    Q.send_email("sasharusin2001@gmail.com");

    return 0;
}