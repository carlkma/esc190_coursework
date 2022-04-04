#include "lab5.h"

int main(){
    //Building the graph in Figure 1
    Graph *gr = initialize_graph();
    add(gr, "Kipling");
    add(gr, "Spadina");
    add(gr, "Yorkdale");
    
    update(gr, "Kipling", "Spadina", 12);
    update(gr, "Spadina", "Yorkdale", 8);
    update(gr, "Spadina", "Bay", 2);
    update(gr, "Spadina", "Union", 4);
    update(gr, "Bay", "Bloor-Yonge", 1);
    update(gr, "Union", "Bloor-Yonge", 3);
    update(gr, "Bloor-Yonge", "Kennedy", 14);
    update(gr, "Bloor-Yonge", "Sheppard-Yonge", 11);
    update(gr, "Sheppard-Yonge", "Finch", 3);
    update(gr, "Sheppard-Yonge", "Don Mills", 6);
    
    print_gr(gr);
    char **r = plan_route(gr, "Kipling", "Don Mills");
    print_route(r, "Kipling");

    //Bye bye Bay!
    disrupt(gr, "Bay");
    print_gr(gr);
    r = plan_route(gr, "Kipling", "Don Mills");
    print_route(r, "Kipling");

    //Hello new stations
    update(gr, "Yorkdale", "Sheppard West", 5);
    update(gr, "Sheppard West", "Sheppard-Yonge", 4);
    print_gr(gr);
    r = plan_route(gr, "Kipling", "Don Mills");
    print_route(r, "Kipling");
    
    free_gr(gr);
    return 0;
}