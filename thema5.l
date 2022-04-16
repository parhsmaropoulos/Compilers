%{
    #include <stdio.h>  
    #include <string.h>     
    char string[] =  "";
    int count = 0;
    int length = 0;
    int shapelength = 0;
    int duplicate = 0;
%}

CHAR [A-H]
POINT {CHAR}
LINE {CHAR}{2}
TRIANGLE {CHAR}{3}
SQUARE {CHAR}{4}
PENTAGON {CHAR}{5}
HEXAGON {CHAR}{6}
HEPTAGON {CHAR}{7}
OCTAGON {CHAR}{8}

NAME "shmeio"|"eutheia"|"trigwno"|"tetragwno"|"pentagwno"|"eksagwno"|"eptagwno"|"oktagwno"
SHAPE " "({POINT}|{LINE}|{TRIANGLE}|{SQUARE}|{PENTAGON}|{HEXAGON}|{HEPTAGON}|{OCTAGON})


%%
"shmeio" {length=1;}
"eutheia" {length=2;}
"trigwno" {length=3;}
"tetragwno" {length=4;}
"pentagwno" {length=5;}
"eksagwno" {length=6;}
"eptagwno" {length=7;}
"oktagwno" {length=8;}

{SHAPE} {shapelength=yyleng-1; duplicate=hasDuplicates(yytext); }


[\n] {

    if (length == shapelength){
        if(duplicate == 0)
        {
            printf("\n Correct expression \n");
        }else{
            printf("\n Invalid exprresion duplicate in shape! \n");
        }
    }else{
        printf("\n Name and shape does not match! \n");
        printf("\n Length %d \n", length);
        printf("\n Shape Length %d \n", shapelength);
    }
    duplicate = 0;
    length = 0;
    shapelength = 0;
}
%%
int hasDuplicates( char string[])
{
     for(int i = 0; i < strlen(string); i++) {  
        count = 1;  
        for(int j = i+1; j < strlen(string); j++) {  
            if(string[i] == string[j] && string[i] != ' ') {  
                count++;  
                //Set string[j] to 0 to avoid printing visited character  
                string[j] = '0';  
            }  
        }  
        //A character is considered as duplicate if count is greater than 1  
        if(count > 1 && string[i] != '0')  
            return 1;  
    }  
    return 0;
}

int yywrap(void){}

int main() {
    yylex();
    printf("\nEnter your expression\n");
    return 0;
}
