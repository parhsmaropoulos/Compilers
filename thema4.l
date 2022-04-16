%{
int count=0;
%}
SYMBOL "+"|"-"|"*"|"%"|"/"
VARIABLE [a-zA-Z]
DIGIT [0-9]
NAME {VARIABLE}{VARIABLE}*
CONST {DIGIT}{DIGIT}*
TERM {NAME}|{DIGIT}{CONST}*
EQ {TERM}({SYMBOL}{TERM})*
EXPR {NAME}"="{EQ}";"
%%
{EXPR} {count++;}

[\n] {
    if(count==0)
    printf("\n Wrong expression %d \n", count);
    else
    printf("\n Correct expresion %d \n", count);
    count = 0;
}
%%
main() {
    char fexpr[100];
    printf("\nEnter your expression\n");
    scanf("%s", fexpr);
    yyin=fopen(fexpr,"r++");
    yylex();
}
