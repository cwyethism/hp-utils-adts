//
//  hp_adts.c
//  HuiMediaPlayer
//
//  Created by zhihui on 27.7.21.
//  Copyright © 2021 Wyeth. All rights reserved.
//

#include "hp_adts.h"

#ifdef __cplusplus
#include <sys/time.h>
#include <stdio.h>
#else
#include <time.h>
#endif

static const int aac_sample_rates[] = {
    96000, 88200, 64000, 48000, 44100, 32000,
    24000, 22050, 16000, 12000, 11025, 8000, 0
};

int match_by_for(int sample_rate) {
    int index = 0;
    for (int i = 0; aac_sample_rates[i] != 0; i++) {
        if (sample_rate == aac_sample_rates[i]){
            index = i;
            break;
        }
    }
    return index;
}

int match_by_switch(int sample_rate) {
    int index = 0;
    switch (sample_rate) {
     case 96000:{
         index = 0;
         break;
     }

     case 88200:{
         index = 1;
         break;
     }

     case 64000:{
         index = 2;
         break;
     }

     case 48000:{
         index = 3;
         break;
     }

     case 44100:{
         index = 4;
         break;
     }

     case 32000:{
         index = 5;
         break;
     }

     case 24000:{
         index = 6;
         break;
     }

     case 22050:{
         index = 7;
         break;
     }

     case 16000:{
         index = 8;
         break;
     }

     case 12000:{
         index = 9;
         break;
     }

     case 11025:{
         index = 10;
         break;
     }

     case 8000:{
         index = 11;
         break;
     }

     case 0:{
         index = 12;
         break;
     }


     default: {
         index = -1;
         break;
     }
    }
    return index;
}

int sample_rate_for(uint32_t index) {
    if (index > 12) {
        return -1;
    }
    return aac_sample_rates[index];
}

int sample_index_for(uint32_t rate) {
    return match_by_switch(rate);
}

int hp_adts_header_parse(uint8_t *frm_buf, hp_aac_format *fmt) {
    if (NULL == frm_buf || NULL == fmt) {
        return -1;
    }
    
    // profile [16-17]
    fmt->profile = frm_buf[2] >> 6;
    
    // sample index [18-21]
    uint8_t sampling_index = (frm_buf[2] & 0x3c) >> 2;
    fmt->sample_rate = sample_rate_for(sampling_index);
    
    // channels [23-25]
    fmt->channels = (frm_buf[2] & 0x01) << 7 | frm_buf[3] >> 6;
    
    // frame size [30-42]
    uint32_t frame_size = 0;
    frame_size |= (((frm_buf[3] & 0x03)) << 11);//high 2 bit
    frame_size |= (frm_buf[4] << 3);//middle 8 bit
    frame_size |= ((frm_buf[5] & 0xe0) >> 5);//low 3bit
    
    // pcket
    fmt->data_size = frame_size - HP_AAC_ADTS_HEADER_SIZE;
    
    return 0;
}

int hp_adts_header_fill(hp_aac_format *fmt, uint8_t *adts_buf) {
    if (NULL == adts_buf || NULL == fmt) {
        return -1;
    }
    
    int sample_rate_index = sample_index_for(fmt->sample_rate);
    if (sample_rate_index < 0) {
        return sample_rate_index;
    }
    
    adts_buf[0] = 0xff;
    adts_buf[1] = 0xf1;
    adts_buf[2] = fmt->profile << 6 | sample_rate_index << 2 | 0x02 | fmt->channels >> 2;
    int aac_frame_length = fmt->data_size + HP_AAC_ADTS_HEADER_SIZE;
    adts_buf[3] = fmt->channels << 6 | aac_frame_length >> 11;
    adts_buf[4] = aac_frame_length >> 3 & 0x000000ff;
    adts_buf[5] = (aac_frame_length & 0x00000007) << 5 | 0x1f;
    adts_buf[6] = 0xfc;
    return 0;
}
