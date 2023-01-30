#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Person {
    char name[57];
    int money;
};

int main(int argc, char *argv[]) {
    int num_of_people = 0;
    scanf("%d", &num_of_people);
    struct Person *people = malloc(sizeof(struct Person) * num_of_people);
    for (int i = 0; i < num_of_people; i++) {
        scanf("%s", people[i].name);
        people[i].money = 0;
    }
    for (int i = 0; i < num_of_people; i++)
    {
        char giver [57];
        scanf("%s", giver);
        int money = 0;
        int num_of_getter = 0;
        scanf("%d %d", &money, &num_of_getter);
        int getter_money = 0, giver_money = 0;
        if (num_of_getter != 0) {
            getter_money = money / num_of_getter;
            giver_money = money - getter_money * num_of_getter;// % num_of_getter
        } else {
            giver_money = money;
        }
        for (int j = 0; j < num_of_people; j++) {
            if (strcmp(giver, people[j].name) == 0) {
                people[j].money -= money;
                people[j].money += giver_money;
            }
        }
        for (int j = 0; j < num_of_getter; j++) {
            char getter[57];
            scanf("%s", getter);
            for (int k = 0; k < num_of_people; k++) {
                if (strcmp(getter, people[k].name) == 0) {
                    people[k].money += getter_money;
                }
            }
        }
    }
    for (int i = 0; i < num_of_people; i++) {
        printf("%s %d\n", people[i].name, people[i].money);
    }
}
//  gcc -o 1 1.c
//  touch 1.c
//  mv src trgt