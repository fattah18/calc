#include <gtk/gtk.h>
#include <gio/gio.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "Ifamth.h"

#define MAX_STACK_SIZE 100 
#define CSS_FILE "style.css"
static GtkWidget *display_entry;

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

long double applyOp(long double a, long double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': if(b-(long)b==0){return power(a, b);}else{return powerfloat(a,b);}
    }
    return 0;
}

long double applyFunction(const char* func, long double x) {
    if (strcmp(func, "sin") == 0) return sinus(x);
    if (strcmp(func, "cos") == 0) return cosin(x);
    if (strcmp(func, "tan") == 0) return tng(x);
    if (strcmp(func, "asin") == 0) return arcsin(x);
    if (strcmp(func, "acos") == 0) return arccosin(x);
    if (strcmp(func, "atan") == 0) return arctng(x);
    if (strcmp(func, "xpo") == 0) return expo(x);
    if (strcmp(func, "ln") == 0) return lnx(x);
    if (strcmp(func, "log") == 0) return logdec(x);
    if (strcmp(func, "sqrt") == 0) return racine(x);
    if (strcmp(func, "cbrt") == 0) return racinecube(x);
    if (strcmp(func, "fact") == 0) return fact(x);
    if (strcmp(func, "decb") == 0) return bin_to_dec((long long)x);
    if (strcmp(func, "bin") == 0) return dec_to_bin((long long)x);
    if (strcmp(func, "deco") == 0) return oct_to_dec((long long)x);
    if (strcmp(func, "oct") == 0) return dec_to_oct((long long)x);
    if (strcmp(func, "cosh") == 0) return cosinh(x);
    if (strcmp(func, "sinh") == 0) return sinush(x);
    if (strcmp(func, "tanh") == 0) return tngh(x);
    if (strcmp(func, "asinh") == 0) return asinush(x);
    if (strcmp(func, "acosh") == 0) return acosinh(x);
    if (strcmp(func, "atanh") == 0) return atngh(x);
    if (strcmp(func, "vabs") == 0) return vabs(x);
    if (strcmp(func, "int") == 0) return intpart(x);
    if (strcmp(func, "frac") == 0) return fracpart(x);
    return 0;
}

long double evaluate(const char* exp) {
    long double values[MAX_STACK_SIZE]; 
    char ops[MAX_STACK_SIZE];           
    int valTop = -1, opTop = -1;
    int i = 0, len = strlen(exp);

    while (i < len) {
        if (exp[i] == ' ') { i++; continue; }

        if (exp[i] == '-' && (i == 0 || exp[i-1] == '(' || strchr("+-*/^", exp[i-1]) != NULL)) {
            ops[++opTop] = '~';
            i++;
            continue;
        }

        if (isdigit(exp[i]) || exp[i] == '.') {
            long double val = 0;
            int dec = 0;
            long double factor = 0.1L;

            while (i < len && (isdigit(exp[i]) || exp[i] == '.')) {
                if (exp[i] == '.') { dec = 1; i++; continue; }
                if (!dec) val = val * 10 + (exp[i] - '0');
                else { val += (exp[i] - '0') * factor; factor /= 10; }
                i++;
            }
            i--;

            if (opTop >= 0 && ops[opTop] == '~') { val = -val; opTop--; }
            values[++valTop] = val;
            i++;
            continue;
        }

        if (i+1 < len && exp[i] == 'p' && exp[i+1] == 'i') {
            long double val = myp;
            if (opTop >= 0 && ops[opTop] == '~') { val = -val; opTop--; }
            values[++valTop] = val;
            i += 2;
            continue;
        }

        if (exp[i] == 'e') {
            long double val = expo(1.0L);
            if (opTop >= 0 && ops[opTop] == '~') { val = -val; opTop--; }
            values[++valTop] = val;
            i++;
            continue;
        }

        if (isalpha(exp[i])) {
            char func[20];
            int f = 0;
            while (i < len && isalpha(exp[i])) func[f++] = exp[i++];
            func[f] = '\0';

            if (exp[i] != '(') { g_printerr("Function error: %s\n", func); return 0; }
            i++; 
            int start = i, level = 1;
            while (i < len && level > 0) {
                if (exp[i] == '(') level++;
                if (exp[i] == ')') level--;
                i++;
            }
            int end = i - 1;
            char sub[200];
            strncpy(sub, exp + start, end-start);
            sub[end-start] = '\0';
            long double arg = evaluate(sub);
            long double val = applyFunction(func, arg);

            if (opTop >= 0 && ops[opTop] == '~') { val = -val; opTop--; }
            values[++valTop] = val;
            continue;
        }

        if (exp[i] == '(') { ops[++opTop] = '('; i++; continue; }
        if (exp[i] == ')') {
            while (opTop >= 0 && ops[opTop] != '(') {
                long double b = values[valTop--];
                long double a = values[valTop--];
                char op = ops[opTop--];
                values[++valTop] = applyOp(a,b,op);
            }
            if (opTop >= 0 && ops[opTop] == '(') {
                opTop--; 
                i++;
                continue;
            } else {
                 g_printerr("Syntax Error: Unmatched closing parenthesis.\n");
                 return 0; 
            }
        }

        if (strchr("+-*/^", exp[i])) {
            while (opTop >= 0 && precedence(ops[opTop]) >= precedence(exp[i])) {
                char op = ops[opTop--];
                long double b = values[valTop--];
                long double a = values[valTop--];
                values[++valTop] = applyOp(a,b,op);
            }
            ops[++opTop] = exp[i];
            i++;
        } else {
            g_printerr("Syntax Error: Unrecognized character: %c.\n", exp[i]);
            return 0;
        }
    }

    while (opTop >= 0) {
        if (ops[opTop] == '(') {
            g_printerr("Syntax Error: Unmatched opening parenthesis.\n");
            return 0;
        }
        char op = ops[opTop--];
        long double b = values[valTop--];
        long double a = values[valTop--];
        values[++valTop] = applyOp(a,b,op);
    }

    if (valTop != 0) {
         g_printerr("Syntax Error: Invalid expression structure.\n");
         return 0;
    }

    return values[valTop];
}


static void on_button_clicked(GtkWidget *widget, gpointer data) {
    const char *label = gtk_button_get_label(GTK_BUTTON(widget));
    const char *current_text = gtk_editable_get_text(GTK_EDITABLE(display_entry));
    
    char new_text[300]; 
    snprintf(new_text, sizeof(new_text), "%s%s", current_text, label);
    
    gtk_editable_set_text(GTK_EDITABLE(display_entry), new_text);
}

static void on_equal_clicked(GtkWidget *widget, gpointer data) {
    const char *expression = gtk_editable_get_text(GTK_EDITABLE(display_entry));
    long double result;
    
    result = evaluate(expression);
    
    char result_str[300];
    snprintf(result_str, sizeof(result_str), "%.15Lg", result);
    
    gtk_editable_set_text(GTK_EDITABLE(display_entry), result_str);
}

static void on_clear_clicked(GtkWidget *widget, gpointer data) {
    gtk_editable_set_text(GTK_EDITABLE(display_entry), "");
}

static void load_css(void) {
    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    
    GFile *file = g_file_new_for_path(CSS_FILE); 
    gtk_css_provider_load_from_file(provider, file);
    
    gtk_style_context_add_provider_for_display(
        display,
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
    
    g_object_unref(provider);
    g_object_unref(file); 
}

static GtkWidget *create_button(const char *label, const char *css_class, GCallback handler, gpointer data) {
    GtkWidget *button = gtk_button_new_with_label(label);
    
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_vexpand(button, TRUE);
    
    if (css_class) {
        gtk_widget_add_css_class(button, css_class);
    }
    
    g_signal_connect(button, "clicked", handler, data);
    return button;
}

static void activate(GtkApplication *app, gpointer user_data) {
    load_css();
    
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "I,F,A scientific calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 600);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_margin_top(main_box, 10);
    gtk_widget_set_margin_bottom(main_box, 10);
    gtk_widget_set_margin_start(main_box, 10);
    gtk_widget_set_margin_end(main_box, 10);
    
    gtk_window_set_child(GTK_WINDOW(window), main_box);

    display_entry = gtk_entry_new();
    gtk_widget_add_css_class(display_entry, "display");
    gtk_editable_set_editable(GTK_EDITABLE(display_entry), TRUE);
    gtk_entry_set_max_length(GTK_ENTRY(display_entry), 299);
    gtk_box_append(GTK_BOX(main_box), display_entry);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 7);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid),TRUE);
    gtk_box_append(GTK_BOX(main_box), grid);

    const char *buttons[] = {
        "C",   "xpo",  "ln",   "cos",  "sin",  "+",
        "tan", "acos", "asin", "atan", "fact", "-",
        "deco","oct",  "decb", "bin",  "log" , "*",
        "sqrt","cbrt", "vabs", "int",  "frac", "/",
        "7",   "8",    "9",    "cosh", "sinh", "^",
        "4",   "5",    "6",    "tanh", "acosh","(",
        "1",   "2",    "3",    "asinh","atanh",")",
        "0",   ".",    "pi",   "e"    ,"*10^", "=",
    };
    
    const char *types[] = {
        "clear", "func", "func", "func", "func", "op", 
        "func", "func", "func", "func", "func", "op", 
        "func", "func", "func", "func", "func", "op",
        "func", "func", "func", "func", "func", "op",
        "numop", "numop", "numop", "func", "func", "op",
        "numop", "numop", "numop", "func", "func", "op",
        "numop", "numop", "numop", "func", "func","op",
        "numop", "numop", "numop", "func", "func","equal",
    };
    
    int row = 0;
    int col = 0;
    for (int i = 0; i < G_N_ELEMENTS(buttons); i++) {
        const char *label = buttons[i];
        if (!label) {
            col++;
            continue; 
        }
        
        GtkWidget *button;
        const char *type = types[i];
        
        if (strcmp(type, "clear") == 0) {
            button = create_button(label, "clear_button", G_CALLBACK(on_clear_clicked), NULL);
        } else if (strcmp(type, "equal") == 0) {
            button = create_button(label, "equal_button", G_CALLBACK(on_equal_clicked), NULL);
        } else if (strcmp(type, "func") == 0) {
            button = create_button(label, "func_button", G_CALLBACK(on_button_clicked), NULL);
        } else if (strcmp(type, "op") == 0) {
            button = create_button(label, "op_button", G_CALLBACK(on_button_clicked), NULL);
        }
        else{
            button = create_button(label, "numop_button", G_CALLBACK(on_button_clicked), NULL);
        }
        gtk_grid_attach(GTK_GRID(grid), button, col, row, 1, 1);
        col++;
        if (col >= 6) {
            col = 0;
            row++;
        }
    }

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {

     GtkApplication *app = gtk_application_new("org.gtk.scientific.calculator", 0);
    
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    
    g_object_unref(app);
    
    return status;
}