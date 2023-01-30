#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

// *** start implement Stack : https://www.geeksforgeeks.org/introduction-to-stack-data-structure-and-algorithm-tutorials/

// A structure to represent a stack
struct Stack {
    int top;
    unsigned capacity;
    int* array;
};

// function to create a stack of given capacity. It initializes size of
// stack as 0
struct Stack* createStack(unsigned capacity)
{
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (int*)malloc(stack->capacity * sizeof(int));
    return stack;
}

// Stack is full when top is equal to the last index
int isFull(struct Stack* stack)
{
    return stack->top == stack->capacity - 1;
}

// Stack is empty when top is equal to -1
int isEmpty(struct Stack* stack)
{
    return stack->top == -1;
}

// Function to add an item to stack.  It increases top by 1
void push(struct Stack* stack, int item)
{
    if (isFull(stack))
        return;
    stack->array[++stack->top] = item;
    // printf("%d pushed to stack\n", item);
}

// Function to remove an item from stack.  It decreases top by 1
int pop(struct Stack* stack)
{
    if (isEmpty(stack))
        return INT_MIN;
    return stack->array[stack->top--];
}

// Function to return the top from stack without removing it
int peek(struct Stack* stack)
{
    if (isEmpty(stack))
        return INT_MIN;
    return stack->array[stack->top];
}

// *** end implement stack

int main(int argc, char *argv[]) {
    char expression[1000];
    scanf("%[^\n]s", expression); // %s --> ta avalin space ro migire
    struct Stack* stack = createStack(1000);
    int index = 0;
    while (expression[index] != '\0') { 
        if (expression[index] == '{') {
            push(stack, -1000); // '{' == -1000 
            index++;
        }
        else if (expression[index] == '}') {
            int sum = 0;
            while (peek(stack) != -1000) {
                sum += pop(stack);
            }
            pop(stack);
            push(stack, sum);
            printf("%d\n", sum);
            index++;
        }
        else if (expression[index] == ' ' || expression[index] == ',')
        {
            // ignore
            index++;
        }
        else // number
        {
            int number = 0;
            while (expression[index] != ',' && expression[index] != '}' && expression[index] != ' ' && expression[index] != '\0') {
                number = number * 10 + (expression[index] - '0');
                index++;
            }
            push(stack, number);
        }
        
    }
    
}