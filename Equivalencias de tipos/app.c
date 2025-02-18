#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef enum { TYPE_VAR, TYPE_INT, TYPE_BOOL, TYPE_FUN } TypeKind;

typedef struct Type {
    TypeKind kind;

    struct Type* instance;
    union {
        int var_id;  
        struct {     
            struct Type* from;
            struct Type* to;
        } fun;
    } u;
} Type;


int next_type_var_id = 0;


Type* new_type_var() {
    Type* t = malloc(sizeof(Type));
    if (!t) { perror("malloc"); exit(1); }
    t->kind = TYPE_VAR;
    t->instance = NULL;
    t->u.var_id = next_type_var_id++;
    return t;
}

Type* make_int() {
    Type* t = malloc(sizeof(Type));
    if (!t) { perror("malloc"); exit(1); }
    t->kind = TYPE_INT;
    t->instance = NULL;
    return t;
}

Type* make_bool() {
    Type* t = malloc(sizeof(Type));
    if (!t) { perror("malloc"); exit(1); }
    t->kind = TYPE_BOOL;
    t->instance = NULL;
    return t;
}


Type* make_fun_type(Type* from, Type* to) {
    Type* t = malloc(sizeof(Type));
    if (!t) { perror("malloc"); exit(1); }
    t->kind = TYPE_FUN;
    t->instance = NULL;
    t->u.fun.from = from;
    t->u.fun.to = to;
    return t;
}


Type* prune(Type* t) {
    if(t->kind == TYPE_VAR && t->instance != NULL) {
        t->instance = prune(t->instance);
        return t->instance;
    }
    return t;
}

bool occurs_in_type(Type* tv, Type* t) {
    t = prune(t);
    if(t == tv) return true;
    if(t->kind == TYPE_FUN)
        return occurs_in_type(tv, t->u.fun.from) || occurs_in_type(tv, t->u.fun.to);
    return false;
}


void unify(Type* t1, Type* t2) {
    t1 = prune(t1);
    t2 = prune(t2);
    
    if(t1->kind == TYPE_VAR) {
        if(t1 != t2) {
            if(occurs_in_type(t1, t2)) {
                printf("Error: ocurre-check fallido.\n");
                exit(1);
            }
            t1->instance = t2;
        }
    } else if(t2->kind == TYPE_VAR) {
        unify(t2, t1);
    } else if(t1->kind == TYPE_INT && t2->kind == TYPE_INT) {
        // ambos son int, OK.
    } else if(t1->kind == TYPE_BOOL && t2->kind == TYPE_BOOL) {
        // ambos son bool, OK.
    } else if(t1->kind == TYPE_FUN && t2->kind == TYPE_FUN) {
        unify(t1->u.fun.from, t2->u.fun.from);
        unify(t1->u.fun.to, t2->u.fun.to);
    } else {
        printf("Error: tipos incompatibles.\n");
        exit(1);
    }
}


void print_type(Type* t) {
    t = prune(t);
    switch(t->kind) {
        case TYPE_VAR:
            printf("t%d", t->u.var_id);
            break;
        case TYPE_INT:
            printf("int");
            break;
        case TYPE_BOOL:
            printf("bool");
            break;
        case TYPE_FUN:
            printf("(");
            print_type(t->u.fun.from);
            printf(" -> ");
            print_type(t->u.fun.to);
            printf(")");
            break;
    }
}

int main() {

    Type* a = new_type_var();
    Type* b = new_type_var();
    Type* fun1 = make_fun_type(a, make_int());
    Type* fun2 = make_fun_type(make_bool(), b);
    
    printf("Antes de unificar:\n");
    printf("fun1: ");
    print_type(fun1);
    printf("\nfun2: ");
    print_type(fun2);
    printf("\n\n");
    
    unify(fun1, fun2);
    
    printf("Después de unificar:\n");
    printf("fun1: ");
    print_type(fun1);
    printf("\nfun2: ");
    print_type(fun2);
    printf("\n");
    
    printf("Tipo de a: ");
    print_type(a);
    printf("\nTipo de b: ");
    print_type(b);
    printf("\n");
    
    free(a);
    free(b);
    free(fun1->u.fun.to); // int
    free(fun1);
    free(fun2);
    
    return 0;
}
