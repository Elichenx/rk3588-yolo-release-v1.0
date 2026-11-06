#ifndef POSTPROCESS_H
#define POSTPROCESS_H

#include <stdint.h>
#include <string>
#include <vector>

#define OBJ_NUM_MAX_SIZE        64
// 临时切换：官方 COCO YOLOv5s，80 类。安全帽 5 类见 model/helmet_labels.txt。
#define OBJ_CLASS_NUM 80
#define LABLE_PATH "../model/coco_80_labels_list.txt"
#define BOX_NUM_SIZE (OBJ_CLASS_NUM+5)
#define MAX_OBJ_BOXS 60

// 官方 YOLOv5 默认置信度是 0.25。原来的 0.65 是安全帽模型用的，COCO 上会漏掉大部分框。
#define BOX_THRESHOLD 0.25
#define NMS_THRESHOLD 0.45


struct box_p
{
    int xmin;
    int ymin;
    int xmax;
    int ymax;
};

struct detect_result_t
{
    char label[32];
    float box_conf;
    box_p box;
};

  // 定义结构体，表示检测结果组
struct detect_result_group_t
{
    int box_count;                          // 框的数量
    detect_result_t result[OBJ_NUM_MAX_SIZE];// 检测结果数组
};  


int post_process(int8_t *output0, int8_t *output1, int8_t *output2, int model_height, int model_width, float box_threshold,
                 float nms_threshold, float scale_w, float scale_h, int pad_left, int pad_top, std::vector<int32_t>& qnt_zps, std::vector<float>& qnt_scales, detect_result_group_t& group);
#endif