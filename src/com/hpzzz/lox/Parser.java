package com.hpzzz.lox;

import java.util.ArrayList;
import java.util.List;

import static com.hpzzz.lox.TokenType.*;

public class Parser {
    private static class ParseError extends RuntimeException {}
    private final List<Token> tokens;
    private int current = 0;

    public Parser(List<Token> tokens) {
        this.tokens = tokens;
    }

    List<Stmt> parse() {
        List<Stmt> statements = new ArrayList<>();
        while (!isAtEnd()) {
            statements.add(statement());
        }
        return statements;
    }

    private Stmt statement() {
        if (match(PRINT)) return printStatement();
        return expressionStatement();
    }

    private Stmt printStatement() {
        Expr value = expression();
        consume(SEMICOLON, "Expect ';' after value.");
        return new Stmt.Print(value);
    }

    private Stmt expressionStatement() {
        Expr expr = expression();
        consume(SEMICOLON, "Expect ';' after expression.");
        return new Stmt.Expression(expr);
    }

    private Expr expression() {
        return comma();
    }

    private Expr comma() {
        Expr expr = ternary();
        while (match(COMMA)) {
            Token coma = previous();
            if (expr == null) {
                error(coma, "Comma operator ',' has no left-hand operand.");
                ternary();
                continue;
            }
            Expr right = ternary();
            expr = new Expr.Binary(expr, coma, right);
        }
        return expr;
    }

    private Expr ternary() {
        Expr expr = equality();
        if (match(QUESTION)) {
            Expr thenBranch = expression();
            if (!check(COLON)) {
                error(peek(), "Expected ':' after the '?' in a ternary expression.");
                return expr; // Recover by returning what we have so far
            }
            consume(COLON, "Expect ':' after expression in ternary operator");
            Expr elseBranch = ternary();
            if (elseBranch == null) {
                error(peek(), "Expected expression after ':' in a ternary expression.");
            }
            expr = new Expr.Ternary(expr, thenBranch, elseBranch);
        }
        return expr;
    }

    private Expr equality() {
        Expr expr = comparison();
        while (match(EQUAL_EQUAL, BANG_EQUAL)) {
            Token operator = previous();
            if (expr == null) {
                error(operator, "Binary operator '" + operator.lexeme + "' has no left-hand operand.");
                comparison();
                continue;
            }
            Expr right = comparison();
            expr = new Expr.Binary(expr, operator, right);
        }

        return expr;
    }

    private boolean match(TokenType... types) {
        for (TokenType type  : types) {
            if (check(type)) {
                advance();
                return true;
            }
        }
        return false;
    }

    private Token advance() {
        if (!isAtEnd()) current++;
        return previous();
    }

    private Token previous() {
        return tokens.get(current - 1);
    }

    private boolean isAtEnd() {
        return peek().type == EOF;
    }

    private Token peek() {
        return tokens.get(current);
    }

    private boolean check(TokenType type) {
        if (isAtEnd()) return false;
        return peek().type == type;
    }

    private Expr comparison() {
        Expr expr = term();
        while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
            Token operator = previous();
            if (expr == null) {
                error(operator, "Binary operator '" + operator.lexeme + "' has no left-hand operand.");
                term();
                continue;
            }
            Expr right = term();
            expr = new Expr.Binary(expr, operator, right);
        }
        return expr;
    }

    private Expr term() {
        Expr expr = factor();
        while (match(PLUS, MINUS)) {
            Token operator = previous();
            if (expr == null) {
                error(operator, "Binary operator '" + operator.lexeme + "' has no left-hand operand.");
                factor();
                continue;
            }
            Expr right = factor();
            expr = new Expr.Binary(expr, operator, right);
        }
        return expr;
    }

    private Expr factor() {
        Expr expr = unary();
        while (match(SLASH, STAR)) {
            Token operator = previous();
            if (expr == null) {
                error(operator, "Binary operator '" + operator.lexeme + "' has no left-hand operand.");
                unary();
                continue;
            }
            Expr right = unary();
            expr = new Expr.Binary(expr, operator, right);
        }
        return expr;
    }

    private Expr unary() {
        if (match(BANG, MINUS)) {
            Token operator = previous();
            Expr right = unary();
            return new Expr.Unary(operator, right);
        }
        return primary();
    }

    private Expr primary() {
        if (match(FALSE)) return new Expr.Literal(false);
        if (match(TRUE)) return new Expr.Literal(true);
        if (match(NIL)) return new Expr.Literal(null);

        if (match(NUMBER, STRING)) {
            return new Expr.Literal(previous().literal);
        }

        if (match(LEFT_PAREN)) {
            Expr expr = expression();
            consume(RIGHT_PAREN, "Expect ')' after expression.");
            return new Expr.Grouping(expr);
        }
        throw error(peek(), "Expect expression.");
    }
    private Token consume(TokenType type, String message) {
        if (check(type)) return advance();
        throw error(peek(), message);
    }
    private ParseError error(Token token, String message) {
        Lox.error(token, message);
        return new ParseError();
    }

    private void synchronize() {
        advance();

        while (!isAtEnd()) {
            if ((previous().type ) == SEMICOLON) return;

            switch (peek().type) {
                case CLASS:
                case FUN:
                case VAR:
                case FOR:
                case IF:
                case WHILE:
                case PRINT:
                case RETURN:
                    return;
            }

            advance();
        }

    }
}
