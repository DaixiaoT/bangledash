#ifndef _FILTER_H_
#define _FILTER_H_

enum FILT_MODE {
    FILT_AVG = 0,
    FILT_MID = 1,
};

void filter_init();

int get_filter_val(int h, int data, FILT_MODE avg);


#endif//_FILTER_H_
