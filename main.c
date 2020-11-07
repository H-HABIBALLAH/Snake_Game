#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>

typedef struct rectan
{
    SDL_Rect rect;
    int orientation;
    struct rectan *next;
} rectangleElement;

typedef struct ori
{
    int x;
    int y;
    int orientation;
    struct ori *next;
} change_Orientation;

typedef struct
{
    change_Orientation *first;
    change_Orientation *last;
} orientation_List;

void draw_Snake(SDL_Renderer *renderer, rectangleElement *head)
{
    rectangleElement *crt = head->next;
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, head);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    while (crt)
    {
        SDL_RenderFillRect(renderer, &crt->rect);
        crt = crt->next;
    }
}

orientation_List *create_List()
{
    orientation_List *l;
    l = (orientation_List *)malloc(sizeof(orientation_List));
    l->first = NULL;
    l->last = NULL;
    return l;
}

rectangleElement *create_Rect()
{
    rectangleElement *rectangle;
    rectangle = (rectangleElement *)malloc(sizeof(rectangleElement));
    rectangle->next = NULL;
    rectangle->rect.h = 30;
    rectangle->rect.w = 30;
    return rectangle;
}

void insert_Rect(rectangleElement *head)
{
    rectangleElement *crt = head;
    while (crt->next)
        crt = crt->next;
    crt->next = create_Rect();
    if (crt->orientation == 2)
    {
        crt->next->rect.y = crt->rect.y - 30;
        crt->next->rect.x = crt->rect.x;
        crt->next->orientation = 2;
    }
    if (crt->orientation == 4)
    {
        crt->next->rect.x = crt->rect.x + 30;
        crt->next->rect.y = crt->rect.y;
        crt->next->orientation = 4;
    }
    if (crt->orientation == 6)
    {
        crt->next->rect.x = crt->rect.x - 30;
        crt->next->rect.y = crt->rect.y;
        crt->next->orientation = 6;
    }
    if (crt->orientation == 8)
    {
        crt->next->rect.y = crt->rect.y + 30;
        crt->next->rect.x = crt->rect.x;
        crt->next->orientation = 8;
    }
}

void move_Snake(rectangleElement *head)
{
    rectangleElement *crt = head;
    while (crt)
    {
        if (crt->orientation == 2)
            crt->rect.y += 1;

        if (crt->orientation == 8)
            crt->rect.y -= 1;

        if (crt->orientation == 4)
            crt->rect.x -= 1;

        if (crt->orientation == 6)
            crt->rect.x += 1;

        crt = crt->next;
    }
}

change_Orientation *create_Ori(rectangleElement *head)
{
    change_Orientation *newOri;
    newOri = (change_Orientation *)malloc(sizeof(change_Orientation));
    newOri->x = head->rect.x;
    newOri->y = head->rect.y;
    newOri->orientation = head->orientation;
    newOri->next = NULL;
    return newOri;
}

void add_Ori(orientation_List *list, rectangleElement *head)
{
    change_Orientation *newOri = create_Ori(head);
    if (list->first == NULL)
    {
        list->first = newOri;
        list->last = newOri;
        return;
    }
    list->last->next = newOri;
    list->last = newOri;
}

void remove_Last_Ori(orientation_List *list)
{
    change_Orientation *crt = list->first;
    if (list->first == list->last)
    {
        list->first = NULL;
        list->last = NULL;
        return;
    }

    while (crt->next != list->last)
        crt = crt->next;

    crt->next = NULL;
    list->last = crt;
    printf("Tail deleted / ");
}

void move_Head_Down(rectangleElement *head, SDL_Rect *food, orientation_List *list, SDL_Renderer *renderer)
{
    SDL_Event windowEvent;
    head->orientation = 2;
    add_Ori(list, head);
    rectangleElement *crt_rec;
    change_Orientation *crt_ori;

    while (1)
    {
        crt_rec = head;
        while (crt_rec)
        {
            crt_ori = list->first;
            while (crt_ori)
            {
                if (crt_rec->rect.x == crt_ori->x && crt_rec->rect.y == crt_ori->y)
                {
                    crt_rec->orientation = crt_ori->orientation;
                    if (!crt_rec->next && !crt_ori->next)
                        remove_Last_Ori(list);
                }
                crt_ori = crt_ori->next;
            }

            crt_rec = crt_rec->next;
        }

        move_Snake(head);

        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT)
                break;
            if (windowEvent.type == SDL_KEYDOWN)
            {
                if (windowEvent.key.keysym.sym == SDLK_LEFT)
                    move_Head_Left(head, food, list, renderer);
                else if (windowEvent.key.keysym.sym == SDLK_RIGHT)
                    move_Head_Right(head, food, list, renderer);
            }
        }
        if (abs((food->x + 10) - (head->rect.x + 15)) < 10 && abs((food->y + 10) - (head->rect.y + 15)) < 10)
        {
            food->x = rand() % 900;
            food->y = rand() % 400;
            insert_Rect(head);
        }
        SDL_SetRenderDrawColor(renderer, 25, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, food);
        draw_Snake(renderer, head);
        SDL_RenderPresent(renderer);
        SDL_Delay(4);
    }
}

void move_Head_Up(rectangleElement *head, SDL_Rect *food, orientation_List *list, SDL_Renderer *renderer)
{
    SDL_Event windowEvent;
    head->orientation = 8;
    add_Ori(list, head);
    rectangleElement *crt_rec;
    change_Orientation *crt_ori;
    while (1)
    {
        crt_rec = head;

        while (crt_rec)
        {
            crt_ori = list->first;
            while (crt_ori)
            {
                if (crt_rec->rect.x == crt_ori->x && crt_rec->rect.y == crt_ori->y)
                {
                    crt_rec->orientation = crt_ori->orientation;
                    if (!crt_rec->next && !crt_ori->next)
                        remove_Last_Ori(list);
                }
                crt_ori = crt_ori->next;
            }

            crt_rec = crt_rec->next;
        }

        move_Snake(head);

        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT)
                break;
            if (windowEvent.type == SDL_KEYDOWN)
            {
                if (windowEvent.key.keysym.sym == SDLK_LEFT)
                    move_Head_Left(head, food, list, renderer);
                else if (windowEvent.key.keysym.sym == SDLK_RIGHT)
                    move_Head_Right(head, food, list, renderer);
            }
        }
        if (abs((food->x + 10) - (head->rect.x + 15)) < 10 && abs((food->y + 10) - (head->rect.y + 15)) < 10)
        {
            food->x = rand() % 900;
            food->y = rand() % 400;
            insert_Rect(head);
        }
        SDL_SetRenderDrawColor(renderer, 25, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, food);
        draw_Snake(renderer, head);
        SDL_RenderPresent(renderer);
        SDL_Delay(4);
    }
}

void move_Head_Left(rectangleElement *head, SDL_Rect *food, orientation_List *list, SDL_Renderer *renderer)
{
    SDL_Event windowEvent;
    head->orientation = 4;
    add_Ori(list, head);
    rectangleElement *crt_rec;
    change_Orientation *crt_ori;
    while (1)
    {
        crt_rec = head;
        while (crt_rec)
        {

            crt_ori = list->first;
            while (crt_ori)
            {

                if (crt_rec->rect.x == crt_ori->x && crt_rec->rect.y == crt_ori->y)
                {
                    crt_rec->orientation = crt_ori->orientation;
                    if (!crt_rec->next && !crt_ori->next)
                        remove_Last_Ori(list);
                }
                crt_ori = crt_ori->next;
            }
            crt_rec = crt_rec->next;
        }

        move_Snake(head);

        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT)
                break;
            if (windowEvent.type == SDL_KEYDOWN)
            {
                if (windowEvent.key.keysym.sym == SDLK_DOWN)
                    move_Head_Down(head, food, list, renderer);
                else if (windowEvent.key.keysym.sym == SDLK_UP)
                    move_Head_Up(head, food, list, renderer);
            }
        }
        if (abs((food->x + 10) - (head->rect.x + 15)) < 10 && abs((food->y + 10) - (head->rect.y + 15)) < 10)
        {
            food->x = rand() % 900;
            food->y = rand() % 400;
            insert_Rect(head);
        }
        SDL_SetRenderDrawColor(renderer, 25, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, food);
        draw_Snake(renderer, head);
        SDL_RenderPresent(renderer);
        SDL_Delay(4);
    }
}

void move_Head_Right(rectangleElement *head, SDL_Rect *food, orientation_List *list, SDL_Renderer *renderer)
{
    SDL_Event windowEvent;
    head->orientation = 6;
    add_Ori(list, head);
    rectangleElement *crt_rec;
    change_Orientation *crt_ori;
    while (1)
    {
        crt_rec = head;
        while (crt_rec)
        {
            crt_ori = list->first;
            while (crt_ori)
            {
                if (crt_rec->rect.x == crt_ori->x && crt_rec->rect.y == crt_ori->y)
                {
                    crt_rec->orientation = crt_ori->orientation;
                    if (!crt_rec->next && !crt_ori->next)
                        remove_Last_Ori(list);
                }
                crt_ori = crt_ori->next;
            }
            crt_rec = crt_rec->next;
        }

        move_Snake(head);

        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT)
                break;
            if (windowEvent.type == SDL_KEYDOWN)
            {
                if (windowEvent.key.keysym.sym == SDLK_DOWN)
                    move_Head_Down(head, food, list, renderer);
                else if (windowEvent.key.keysym.sym == SDLK_UP)
                    move_Head_Up(head, food, list, renderer);
            }
        }
        if (abs((food->x + 10) - (head->rect.x + 15)) < 10 && abs((food->y + 10) - (head->rect.y + 15)) < 10)
        {
            food->x = rand() % 900;
            food->y = rand() % 400;
            insert_Rect(head);
        }
        SDL_SetRenderDrawColor(renderer, 25, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, food);
        draw_Snake(renderer, head);
        SDL_RenderPresent(renderer);
        SDL_Delay(4);
    }
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Unable to initialize SDL: %s", SDL_GetError());
        exit(-1);
    }
    SDL_Window *window;
    window = SDL_CreateWindow("Hamza", 100, 100, 1000, 500, SDL_WINDOW_OPENGL);
    if (window == NULL)
    {
        printf("error");
        exit(-1);
    }
    SDL_Event windowEvent;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    rectangleElement *head;
    head->rect.x = 150;
    head->rect.y = 150;
    head->rect.h = 30;
    head->rect.w = 30;
    head->next = NULL;
    head->orientation = 6;

    SDL_Rect food = {200, 200, 20, 20};

    orientation_List *list = create_List();

    while (1)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT)
                break;
            if (windowEvent.type == SDL_KEYDOWN)
            {
                if (windowEvent.key.keysym.sym == SDLK_DOWN)
                    move_Head_Down(head, &food, list, renderer);
                else if (windowEvent.key.keysym.sym == SDLK_UP)
                    move_Head_Up(head, &food, list, renderer);
                else if (windowEvent.key.keysym.sym == SDLK_LEFT)
                    move_Head_Left(head, &food, list, renderer);
                else if (windowEvent.key.keysym.sym == SDLK_RIGHT)
                    move_Head_Right(head, &food, list, renderer);
            }
        }
        head->rect.x += 1;
        SDL_SetRenderDrawColor(renderer, 25, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &food);
        draw_Snake(renderer, head);
        SDL_RenderPresent(renderer);
        SDL_Delay(4);
    }
}
