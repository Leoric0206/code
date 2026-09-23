#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

double val[1000];
char op[1000];
int vp=0,opp=0;
void pushv(double x){val[vp++]=x;}
double popv(){return val[--vp];}
void pusho(char c){op[opp++]=c;}
char popo(){return op[--opp];}

int prec(char o)
{
    switch(o){
        case 's':
		case 'c':
		case 't':
		case 'l':return 5;
        case '^':return 4;
        case '*':
		case '/':return 3;
        default:return 2;
    }
}
int leftAssoc(char o){return o=='+'||o=='-'||o=='*'||o=='/';}

void apply(char o)
{
    double a,b=popv();
    switch(o){
        case '+':a=popv(); pushv(a+b); break;
        case '-':a=popv(); pushv(a-b); break;
        case '*':a=popv(); pushv(a*b); break;
        case '/':a=popv(); pushv(a/b); break;
        case '^':a=popv(); pushv(pow(a,b)); break;
        case 's':pushv(sin(b)); break;
        case 'c':pushv(cos(b)); break;
        case 't':pushv(tan(b)); break;
        case 'l':pushv(log(b)); break;
    }
}

int main()
{
	char s[10000];
    printf("Input:");
	fgets(s,sizeof s,stdin);
    char *p=s; int hasOperand=0;
    while(*p)
	{
        if(isspace(*p)){p++; continue;}
        if(isdigit(*p)||*p=='.')
		{
            char *end;
			pushv(strtod(p,&end));
			p=end;
			hasOperand=1;
			continue;
        }
        if(*p=='(')
		{
			pusho('(');
			p++;
			hasOperand=0;
			continue;
		}
        if(*p==')')
		{
            while(opp>0 && op[opp-1]!='(') apply(popo());
            popo();
			p++;
			hasOperand=1;
			continue;
        }
        if(isalpha(*p))
		{
            char f;
            if(strncmp(p,"sin",3)==0) f='s';
            else if(strncmp(p,"cos",3)==0) f='c';
            else if(strncmp(p,"tan",3)==0) f='t';
            else if(strncmp(p,"log",3)==0) f='l';
            else {printf("unknown function\n"); return 1;}
            p += 3;
            while(opp>0 && op[opp-1]!='(' && prec(op[opp-1]) > prec(f)) apply(popo());
            pusho(f); hasOperand = 0; continue;
        }
        if(strchr("+-*/^",*p))
		{
            char o=*p;
            if((o=='+'||o=='-') && !hasOperand)
                pushv(0.0);
            while(opp>0 && op[opp-1]!='(' && (prec(op[opp-1])>prec(o)||(prec(op[opp-1])==prec(o) && leftAssoc(o))))
                apply(popo());
            pusho(o);
			p++;
			hasOperand=0;
			continue;
        }
        p++;
    }
    while(opp>0) apply(popo());
    printf("Result:%g\n",popv());
    return 0;
}