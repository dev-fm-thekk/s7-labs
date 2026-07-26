#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100

typedef enum {
    KEYWORD,
    IDENTIFIER,
    OPERATOR,
    SPECIAL
} TOKEN_TYPE;

struct token {
    int id;
    char lexeme[50];
    TOKEN_TYPE token_type;
};

const char *keywords[] = {
    "int", "float", "char", "if", "else",
    "while", "return", "break", "continue", "for"
};

char operators[] = {'+', '-', '*', '/', '>', '<', '^'};
char specials[] = {'(', ')', '{', '}', ';'};

int keywords_count = 10;
int operator_count = 7;
int specials_count = 5;


int isKeyword(char lexeme[]) {
    for (int i = 0; i < keywords_count; i++) {
        if (strcmp(keywords[i], lexeme) == 0)
            return 1;
    }
    return 0;
}

int isOperatorChar(char ch) {
    for (int i = 0; i < operator_count; i++) {
        if (operators[i] == ch)
            return 1;
    }
    return 0;
}

int isBracketOrSemi(char ch) {
    for (int i = 0; i < specials_count; i++) {
        if (specials[i] == ch)
            return 1;
    }
    return 0;
}

void printTokens(struct token tokens[], int tokenCount) {
    printf("Total Tokens: %d\n\n", tokenCount);

    for (int i = 0; i < tokenCount; i++) {

        char *type;

        switch (tokens[i].token_type) {
            case KEYWORD:
                type = "KEYWORD";
                break;
            case IDENTIFIER:
                type = "IDENTIFIER";
                break;
            case OPERATOR:
                type = "OPERATOR";
                break;
            case SPECIAL:
                type = "SPECIAL";
                break;
            default:
                type = "UNKNOWN";
        }

        printf("ID      : %d\n", tokens[i].id);
        printf("Lexeme  : %s\n", tokens[i].lexeme);
        printf("Type    : %s\n\n", type);
    }
}

int main() {

    FILE *fp = fopen("test.c", "r");

    if (fp == NULL) {
        perror("File not found");
        return 1;
    }

    struct token tokens[MAX_TOKENS];
    int token_count = 0;
    int ch;

    while ((ch = getc(fp)) != EOF) {

        if (isspace(ch))
            continue;

        // Identifier / Keyword
        if (isalpha(ch) || ch == '_') {

            char lexeme[50];
            int pos = 0;

            lexeme[pos++] = ch;

            while ((ch = getc(fp)) != EOF &&
                   (isalnum(ch) || ch == '_')) {

                lexeme[pos++] = ch;
            }

            lexeme[pos] = '\0';

            if (ch != EOF)
                ungetc(ch, fp);

            struct token newToken;
            newToken.id = token_count;

            strcpy(newToken.lexeme, lexeme);

            if (isKeyword(lexeme))
                newToken.token_type = KEYWORD;
            else
                newToken.token_type = IDENTIFIER;

            tokens[token_count++] = newToken;
        }

        // Operator
        else if (isOperatorChar(ch)) {

            struct token newToken;

            newToken.id = token_count;
            newToken.lexeme[0] = ch;
            newToken.lexeme[1] = '\0';
            newToken.token_type = OPERATOR;

            tokens[token_count++] = newToken;
        }

        // Special Symbols
        else if (isBracketOrSemi(ch)) {

            struct token newToken;

            newToken.id = token_count;
            newToken.lexeme[0] = ch;
            newToken.lexeme[1] = '\0';
            newToken.token_type = SPECIAL;

            tokens[token_count++] = newToken;
        }
    }

    printTokens(tokens, token_count);

    fclose(fp);

    return 0;
}