#include <iostream>
#include <cstdio>
#include <cassert>
#include <cmath>
using std::cin;
using std::cout;
using std::endl;

#include "super_define.h"
#include "dump.h"

#include "Diff_Tree.h"

int main() {

    Diff_Tree Q;

    Q.get_from_file(X_("equation.txt"));

    Q.open_tex();

    Q.put_tex(Q.head_);

    Q.conv_const(Q.head_);

    Q.put_tex(Q.head_);

//    Q.dump_picture();
    Q.close_tex();

    return 0;
}