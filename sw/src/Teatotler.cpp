#include <cr_section_macros.h>

int main(void) {
    volatile static int i = 0 ;
    while(1) {
        i++ ;
    }

    return 0 ;
}
