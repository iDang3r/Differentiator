//
// Created by Александр on 16.11.2019.
//

#ifndef DIFFERENTIATOR_DIFF_TREE_H
#define DIFFERENTIATOR_DIFF_TREE_H

#include "my_file_lib.h"
#include "Tree.h"
#include "super_define.h"

const char open_bracket  = '(';
const char close_bracket = ')';

const double e  = 2.718282;
const double pi = 3.141593;

namespace type {
    enum {
        num    = 0,
        func   = 1,
        var    = 2,
        constt = 3,
    };
}

#define ADD(l, r) create(1, add_, l, r)
#define SUB(l, r) create(1, sub_, l, r)
#define MUL(l, r) create(1, mul_, l, r)
#define DIV(l, r) create(1, div_, l, r)
#define LOG(l, r) create(1, log_, l, r)
#define POW(l, r) create(1, pow_, l, r)
#define SIN(x)    create(1, sin_, nullptr, x)
#define COS(x)    create(1, cos_, nullptr, x)
#define LN(x)     create(1, ln_, nullptr, x)
#define SQRT(x)   create(1, sqrt_, nullptr, x)
//#define NUM(x)    create(var, x)

enum functions {
    add_  = 1,
    sub_  = 2,
    mul_  = 3,
    div_  = 4,  // bin
    log_  = 5,
    sqrt_ = 6,
    pow_  = 7,

    sin_  = 20,
    cos_  = 21,  // mono
    ln_   = 22,
};

namespace easy {
    int size = 5;

    const char* content[] {
        "Это же просто смешно: ",
        "Даже Дане с Андреем понятно: ",
        "Моя младшая сестра такое с закрытыми глазами проворачивает: ",
        "Такое даже на заборах писать неприлично: ",
        "Извините, а когда думать начинать? "
    };

}

namespace hard {
    int size = 9;

    const char* content[] {
        "Такие преобразования должны запретить, они разрушают кору головного мозга: ",
        "Ну, Андрей такое точно не потянет: ",
        "Илон Маск для такого целую компанию создал: ",
        "Я звонил Турсуной Закировой, победительнице битвы экстрасенсов 2005, она подсказала такое преобразование равно: ",
        "Это я своровал у Wolfram'a, ибо нерешаемо обычным разумом: ",
        "Уму не постижимо! Это же: ",
        "Даже не представляю как такое объяснить, чёртова магия: ",
        "Разложил карты рядом со свечками и мантрами, получил это: ",
        "Такое не разбирают даже в мат. группе! Шок контент, смотреть без СМС: "
    };
}

union uni_data {
    double value_;
    int func_num_;
    char var_;

    uni_data(double value = 0) : value_(value) {}
    uni_data(int func_num) : func_num_(func_num) {}
    uni_data(char var) : var_(var) {}

};

struct Diff_Tree : Tree<uni_data> {

    FILE* TeX = nullptr;

    char diff_by = 'x';

    bool stop_tex = false;

    node<uni_data>* print_tex = nullptr;

    // METHODS

    void open_tex() {
        TeX = fopen("Diff.tex", "w");

        if (TeX == nullptr) {
            dump(DUMP_INFO, "file for TeX was not opened");
            assert(ERROR);
        }

        fprintf(TeX, "\\documentclass[a4paper,12pt]{article}\n");
        fprintf(TeX, "\\usepackage{cmap}\n");
        fprintf(TeX, "\\usepackage[T2A]{fontenc}\n");
        fprintf(TeX, "\\usepackage[utf8]{inputenc}\n");
        fprintf(TeX, "\\usepackage[english,russian]{babel}\n");
        fprintf(TeX, "\\usepackage[14pt]{extsizes}\n");
        fprintf(TeX, "\\author{Rusin.AI@phystech.edu}\n");
        fprintf(TeX, "\\title{Differentiations}\n");
        fprintf(TeX, "\\date{\\today}\n");
        fprintf(TeX, "\\begin{document}\n");
        fprintf(TeX, "\\maketitle\n");
        fprintf(TeX, "\n");
        fprintf(TeX, "\n");

    }

    void close_tex() {
        fprintf(TeX, "\\end{document}\n");

        fclose(TeX);

        system("ls");

        system("pdflatex Diff.tex");
        system("open Diff.pdf");
    }

    void put_tex_rec(node<uni_data>* el, int priority = 0) {

        if (el->type_ == type::num) {
            fprintf(TeX, " %g ", el->data_.value_);
            return;
        }

        if (el->type_ == type::var) {
            fprintf(TeX, " %c ", el->data_.var_);
            return;
        }

        if (el->type_ == type::constt) {
            if (el->data_.var_ == 'e')
                fprintf(TeX, " e ");
            if (el->data_.var_ == 'p')
                fprintf(TeX, " \\pi ");
            return;
        }

        if (el->type_ == type::func) {

            int c = el->data_.func_num_;

            if ((c == add_ || c == sub_) && priority)
                fprintf(TeX, "(");

            if (el->left_ && c != div_ && c != log_ && c != mul_)
                put_tex_rec(el->left_, (3 <= el->data_.func_num_ && el->data_.func_num_ <= 4) ? 1 : 0);

            switch (el->data_.func_num_) {
                case add_:
                    fprintf(TeX, "+");
                    break;
                case sub_:
                    fprintf(TeX, "-");
                    break;
                case mul_:
                    if (el->left_->type_ == type::num && is_zero(el->left_->data_.value_ + 1)) {
                        fprintf(TeX, " -");
                    } else {
                        put_tex_rec(el->left_, 1);
                        fprintf(TeX, " \\cdot ");
                    }

                    break;
                case div_:
                    fprintf(TeX, "\\frac{");
                    put_tex_rec(el->left_);
                    fprintf(TeX, "}");
                    fprintf(TeX, "{");
                    put_tex_rec(el->right_);
                    fprintf(TeX, "}");
                    return;
                case sin_:
                    fprintf(TeX, "sin(");
                    put_tex_rec(el->right_);
                    fprintf(TeX, ")");
                    return;
                case cos_:
                    fprintf(TeX, "cos(");
                    put_tex_rec(el->right_);
                    fprintf(TeX, ")");
                    return;
                case log_:
                    fprintf(TeX, "log_{");
                    put_tex_rec(el->left_);
                    fprintf(TeX, "}");
                    fprintf(TeX, "(");
                    put_tex_rec(el->right_);
                    fprintf(TeX, ")");
                    return;
                case ln_:
                    fprintf(TeX, "ln(");
                    put_tex_rec(el->right_);
                    fprintf(TeX, ")");
                    return;
                case pow_:
                    fprintf(TeX, "^{");
                    put_tex_rec(el->right_);
                    fprintf(TeX, "}");
                    return;
                case sqrt_:
                    fprintf(TeX, "\\sqrt{");
                    put_tex_rec(el->right_);
                    fprintf(TeX, "}");
                    return;
            }

            put_tex_rec(el->right_, (3 <= el->data_.func_num_ && el->data_.func_num_ <= 4) ? 1 : 0);

            if ((c == add_ || c == sub_) && priority)
                fprintf(TeX, ")");
        }

    }

    void put_tex(node<uni_data>* el, int d = 0) {
        if (stop_tex)
            return;

        fprintf(TeX, "$ ");

        if (d)
            fprintf(TeX, "(");
        put_tex_rec(el);
        if (d)
            fprintf(TeX, ")'");

        fprintf(TeX, " $\n\n");
    }

    void tex_section(const char* s) {
        if (stop_tex)
            return;
        fprintf(TeX, "\\section{");
        fprintf(TeX, s);
        fprintf(TeX, "}\n\n");
    }

    void tex(const char* s, int same_str = 0) {
        if (stop_tex)
            return;
        if (!same_str)
            fprintf(TeX, "\\par ");
        fprintf(TeX, s);
        fprintf(TeX, "\n");
    }

    char* skip_spaces(char* buff) {
        int n = 0;
        sscanf(buff, "%*[ \t]%n", &n);
        return buff + n;
    }

    char* build_tree_rec(char* buff, node<uni_data>* *el, char* str_buff) {
        assert(el != nullptr);
        assert(buff != nullptr);

//        cout << "new _ node" << endl;

        buff = skip_spaces(buff);

        assert(*buff == open_bracket);
        buff++;
        int n = 0;

        *el = make_node(uni_data());

        buff = skip_spaces(buff);

        if (*buff == open_bracket)
            buff = build_tree_rec(buff, &((*el)->left_), str_buff);

        buff = skip_spaces(buff);

        sscanf(buff, "%[^ ()]%n", str_buff, &n);
        buff += n;

        if (strcmp(str_buff, "+") == 0) {

            (*el)->data_.func_num_ = add_;

        } else if (strcmp(str_buff, "-") == 0) {

            (*el)->data_.func_num_ = sub_;

        } else if (strcmp(str_buff, "*") == 0) {

            (*el)->data_.func_num_ = mul_;

        } else if (strcmp(str_buff, "/") == 0) {

            (*el)->data_.func_num_ = div_;

        } else if (strcmp(str_buff, "sin") == 0) {

            (*el)->data_.func_num_ = sin_;

        } else if (strcmp(str_buff, "cos") == 0) {

            (*el)->data_.func_num_ = cos_;

        } else if (strcmp(str_buff, "log") == 0) {

            (*el)->data_.func_num_ = log_;

        } else if (strcmp(str_buff, "ln") == 0) {

            (*el)->data_.func_num_ = ln_;

        } else if (strcmp(str_buff, "^") == 0) {

            (*el)->data_.func_num_ = pow_;

        } else if (strcmp(str_buff, "sqrt") == 0) {

            (*el)->data_.func_num_ = sqrt_;

        } else if (strcmp(str_buff, "e") == 0 || strcmp(str_buff, "E") == 0) {

            (*el)->type_ = type::constt;
            (*el)->data_.var_ = 'e';

        } else if (strcmp(str_buff, "pi") == 0 || strcmp(str_buff, "Pi") == 0 || strcmp(str_buff, "PI") == 0) {

            (*el)->type_ = type::constt;
            (*el)->data_.var_ = 'p';

        } else if (strlen(str_buff) == 1 && isalpha(*str_buff)) {

            (*el)->type_ = type::var;
            (*el)->data_.var_ = *str_buff;

        } else {

            (*el)->type_ = type::num;
            (*el)->data_.value_ = atof(str_buff);

        }

        buff = skip_spaces(buff);

        if (*buff == open_bracket)
            buff = build_tree_rec(buff, &((*el)->right_), str_buff);

        buff = skip_spaces(buff);

        assert(*buff == close_bracket);
        return buff + 1;
    }

    void get_from_file(const char* file_name) {

        char* buff = nullptr;
        size_t buff_size = get_buffer(file_name, &buff);

        char str_buff[128] = {};

        build_tree_rec(buff, &head_, str_buff);

        free(buff);
    }

    void dump_picture_rec(FILE* out, node<uni_data>* el, int last = -1) {

        fprintf(out, "class %p {\n", el);

        if (last == 1) {
            fprintf(out, "right\n");
        } else if (last == 0) {
            fprintf(out, "left\n");
        } else {
            fprintf(out, "Default\n");
        }

        fprintf(out, "--\n");

        {  // MAIN CONTENT PRINTING

            if (el->type_ == type::num) {

                fprintf(out, "%lf\n", el->data_.value_);

            } else if (el->type_ == type::func) {

                switch (el->data_.func_num_) {
                    case add_:
                        fprintf(out, "+\n");
                        break;
                    case sub_:
                        fprintf(out, "-\n");
                        break;
                    case mul_:
                        fprintf(out, "*\n");
                        break;
                    case div_:
                        fprintf(out, "/\n");
                        break;
                    case sin_:
                        fprintf(out, "sin\n");
                        break;
                    case cos_:
                        fprintf(out, "cos\n");
                        break;
                    case log_:
                        fprintf(out, "log\n");
                        break;
                    case ln_:
                        fprintf(out, "ln\n");
                        break;
                    case pow_:
                        fprintf(out, "^\n");
                        break;
                    case sqrt_:
                        fprintf(out, "√\n");
                        break;
                }

            } else if (el->type_ == type::var) {

                fprintf(out, "%c\n", el->data_.var_);

            } else { // type::constt

                if (el->data_.var_ == 'e') {
                    fprintf(out, "e\n");
                } else if (el->data_.var_ == 'p') {
                    fprintf(out, "pi\n");
                }

            }

        }


        if (el->right_) {
            fprintf(out, "--\n");
            fprintf(out, "right: %p\n", el->right_);
        }

        if (el->left_) {
            fprintf(out, "--\n");
            fprintf(out, "left: %p\n", el->left_);
        }

        fprintf(out, "}\n");

        if (el->right_) {
            fprintf(out, "%p -down-> %p\n", el, el->right_);

            dump_picture_rec(out, el->right_, 1);
        }

        if (el->left_) {
            fprintf(out, "%p -down-> %p\n", el, el->left_);

            dump_picture_rec(out, el->left_, 0);
        }
    }

    void dump_picture() {

        FILE* out = fopen("/Users/alex/Desktop/pictures/Diff_uml.pu", "w");

        if (out == nullptr) {
            dump(DUMP_INFO, "file was not opened");
            return;
        }

        fprintf(out, "@startuml\n !define DARKORANGE\n !include style.puml\n class head_\n");

        fprintf(out, "head_ -down-> %p\n", head_);

        dump_picture_rec(out, head_);

        fprintf(out, "@enduml\n");
        fclose(out);

        system("java -jar --illegal-access=warn /Users/alex/plantuml.jar -tsvg /Users/alex/Desktop/pictures/Diff_uml.pu");
        system("open /Users/alex/Desktop/pictures/Diff_uml.svg");

//        system("java -jar --illegal-access=warn /Users/alex/plantuml.jar /Users/alex/Desktop/pictures/Diff_uml.pu");
//        system("open /Users/alex/Desktop/pictures/Diff_uml.png");
    }

    int conv_const_rec(node<uni_data>* el) {
        assert(el);


        if (el->type_ == type::func && el->right_->type_ == type::num && ((el->data_.func_num_ == sin_ ||
                el->data_.func_num_ == cos_ || el->data_.func_num_ == sqrt_ || el->data_.func_num_ == ln_) || el->left_->type_ == type::num)) {

            tex("Попробуем преобразовать....");
            put_tex(el);

            el->type_ = 0;

            switch (el->data_.func_num_) {
                case add_:
                    el->data_.value_ = el->left_->data_.value_ + el->right_->data_.value_;
                    break;
                case sub_:
                    el->data_.value_ = el->left_->data_.value_ - el->right_->data_.value_;
                    break;
                case mul_:
                    el->data_.value_ = el->left_->data_.value_ * el->right_->data_.value_;
                    break;
                case div_:
                    el->data_.value_ = el->left_->data_.value_ / el->right_->data_.value_;
                    break;
                case sin_:
                    el->data_.value_ = sin(el->right_->data_.value_);
                    break;
                case cos_:
                    el->data_.value_ = cos(el->right_->data_.value_);
                    break;
                case log_:
                    el->data_.value_ = log(el->right_->data_.value_) / log(el->left_->data_.value_);
                    break;
                case ln_:
                    el->data_.value_ = log(el->right_->data_.value_);
                    break;
                case pow_:
                    el->data_.value_ = pow(el->left_->data_.value_, el->right_->data_.value_);
                    break;
                case sqrt_:
                    el->data_.value_ = sqrt(el->right_->data_.value_);
                    break;
            }

            free_tree(el->right_);
            if (el->left_)
                free_tree(el->left_);

            el->left_  = nullptr;
            el->right_ = nullptr;

            tex(hard::content[rand() % hard::size]);
            put_tex(el);

            tex("------");

            return 1;
        }

        node<uni_data>* tmp = nullptr;

        if (el->type_ == type::func) {

            switch (el->data_.func_num_) {
                case add_:

                    if (el->left_->type_ == type::num && is_zero(el->left_->data_.value_)) {

                        tmp = el->right_;

                        *el = *(el->right_);

                        delete tmp;
                        return 1;
                    }

                    if (el->right_->type_ == type::num && is_zero(el->right_->data_.value_)) {

                        tmp = el->left_;

                        *el = *(el->left_);

                        delete tmp;
                        return 1;
                    }

                    if (el->left_->type_ >= 2 && el->left_->type_ == el->right_->type_ && el->left_->data_.var_ == el->right_->data_.var_) {

                        el->data_.func_num_ = sub_; // TODO
                        el->left_->type_ = type::num;
                        el->left_->data_.value_ = 2.;

                        return 1;
                    }

                    break;
                case sub_:

                    break;
                case mul_:

                    if ((el->right_->type_ == type::num && is_zero(el->right_->data_.value_)) ||
                            (el->left_->type_ == type::num && is_zero(el->left_->data_.value_))) {
                        free_tree(el->left_);
                        free_tree(el->right_);
                        *el = node<uni_data>(0, 0.);

                        return 1;
                    }

                    if (el->right_->type_ == type::num && is_zero(el->right_->data_.value_ - 1)) {
                        delete el->right_;

                        node<uni_data>* tmp = el->left_;

                        *el = *(el->left_);

                        delete tmp;
                        return 1;
                    }

                    if (el->left_->type_ == type::num && is_zero(el->left_->data_.value_ - 1)) {
                        delete el->left_;

                        node<uni_data>* tmp = el->right_;

                        *el = *(el->right_);

                        delete tmp;
                        return 1;
                    }

                    break;
                case div_:

                    break;
                case pow_:

                    break;

            }

        }

        int cx = 0;

        if (el->left_) {
            while (conv_const_rec(el->left_))
                cx++;
        }

        if (el->right_) {
            while (conv_const_rec(el->right_))
                cx++;
        }

        return cx;
    }

    void conv_const(node<uni_data>* el) {
        assert(el != nullptr);

        while (conv_const_rec(el));

    }

    int counter(node<uni_data>* el) {
        int s = 1;
        if (el->left_)
            s += counter(el->left_);
        if (el->right_)
            s += counter(el->right_);
        return s;
    }

    node<uni_data>* create(int type, uni_data val, node<uni_data>* left = nullptr, node<uni_data>* right = nullptr) {
        return new node<uni_data>(type, val, left, right);
    }

    node<uni_data>* copy(node<uni_data>* el) {
        node<uni_data>* new_el = new node<uni_data>(*el);
        if (el->left_)
            new_el->left_ = copy(el->left_);
        if (el->right_)
            new_el->right_ = copy(el->right_);
        return new_el;
    }

    void d(node<uni_data>* el) {
        assert(el != nullptr);

        if (el->type_ == type::num || el->type_ == type::constt || el->type_ == type::var) {

            tex("Попробуем преобразовать....");
            put_tex(el, 1);

            el->type_ = type::num;

            if (el->data_.var_ == diff_by) {
                el->data_.value_ = 1.;
            } else {
                el->data_.value_ = 0.;
            }

            tex(hard::content[rand() % hard::size]);
            put_tex(el);

            tex("------");

            return;
        }

        assert(el->type_ == type::func);

        node<uni_data>* tmp = nullptr;

        if (print_tex == nullptr && counter(el) >= 5) {
            tex("Попробуем преобразовать....");
            put_tex(el, 1);
            print_tex = el;
            stop_tex = true;
        }

        switch (el->data_.func_num_) {
            case add_:
                d(el->left_);
                d(el->right_);
                break;

            case sub_:
                d(el->left_);
                d(el->right_);
                break;

            case mul_:

                tmp = ADD(MUL(     el->left_,       el->right_),
                          MUL(copy(el->left_), copy(el->right_)));

                *el = *tmp;

                delete tmp;

                d(el->left_->left_);
                d(el->right_->right_);

                break;

            case div_:

                tmp = DIV(SUB(MUL(el->left_, el->right_), MUL(copy(el->left_), copy(el->right_))),
                        //-----------------------------------------------------------------------
                          POW(copy(el->right_), create(0, 2.)));

                *el = *tmp;

                delete tmp;

                d(el->left_->left_->left_);
                d(el->left_->right_->right_);

                break;

            case sin_:

                tmp = MUL(COS(copy(el->right_)), el->right_);

                *el = *tmp;

                delete tmp;

                d(el->right_);

                break;

            case cos_:

                tmp = MUL(MUL(create(0, -1.), SIN(copy(el->right_))), el->right_);

                *el = *tmp;

                delete tmp;

                d(el->right_);

                break;

            case log_:

                tmp = DIV(copy(el->right_), MUL(el->right_, LN(el->left_)));

                *el = *tmp;

                delete tmp;

                d(el->left_);

                break;

            case ln_:

                tmp = DIV(copy(el->right_), el->right_);

                *el = *tmp;

                delete tmp;

                d(el->left_);

                break;

            case pow_:

                tmp = MUL(ADD(MUL(DIV(create(0, 1.), copy(el->left_)), MUL(copy(el->left_), copy(el->right_))), MUL(LN(copy(el->left_)), copy(el->right_))), POW(el->left_, el->right_));

                *el = *tmp;

                delete tmp;

                d(el->left_->left_->right_->left_);
                d(el->left_->right_->right_);

                break;

            case sqrt_:

                tmp = DIV(copy(el->right_), MUL(create(0, 2.), SQRT(el->right_)));

                *el = *tmp;

                delete tmp;

                d(el->left_);

                break;
        }

        if (print_tex == el) {
            stop_tex = false;
            tex(easy::content[rand() % easy::size]);
            put_tex(el);
            tex("------");
        }

    }

    void send_email(const char* email) {

        char* s1 = concat("mail ", email);
        char* s2 = concat(s1, " -r Rusin.AI@phystech.edu -A /Users/alex/Desktop/Clion/Differentiator/cmake-build-debug/Diff.pdf < /Users/alex/Desktop/text_for_email.txt");

        free(s1);

        system(s2);

        free(s2);
    }

    void put_var_rec(node<uni_data>* el, char var, double value) {

        if (el->type_ == type::var && el->data_.var_ == var) {
            el->type_ = type::num;
            el->data_.value_ = value;
            return;
        }

        if (el->left_)
            put_var_rec(el->left_, var, value);

        if (el->right_)
            put_var_rec(el->right_, var, value);
    }

    void put_var(char var, double value) {
        put_var_rec(head_, var, value);
    }
};




#endif //DIFFERENTIATOR_DIFF_TREE_H
