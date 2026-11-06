#ifndef YOLOV5S_H
#define YOLOV5S_H

#include <iostream>
#include <string.h>
#include <vector>
#include <cstddef>

#include "3rdparty/librknn_api/include/rknn_api.h"

#include "RgaUtils.h"
#include "im2d.h"
#include "rga.h"

#include "post_process.h"



class Yolov5s
{
private:
    rknn_context context;
    unsigned int model_size;

    rknn_input_output_num num_tensors;
    std::vector<rknn_tensor_attr> input_attrs;
    std::vector<rknn_tensor_attr> output_attrs;

    unsigned char *model_data;
    unsigned char *load_model(const char *model_path, unsigned int &model_size);

    // 持久化 RKNN IO 缓冲：构造时分配一次，整个生命周期复用，避免每帧 alloc/free 引起 cache miss
    rknn_tensor_mem *input_mem_{nullptr};
    std::vector<rknn_tensor_mem *> output_mems_;

public:
    Yolov5s(const char *model_path, int npu_index, int img_w = 640, int img_h = 480);
    ~Yolov5s();

    // 模型要求的输入尺寸（由 rknn_query 填充）
    int model_height;
    int model_width;

    // 摄像头原始尺寸（与采集分辨率一致，由构造函数传入）
    int img_height;
    int img_width;

    // 转换后的尺寸
    int resized_height;
    int resized_width;

    // 转换因子
    float scale;

    // x与y的偏移量
    int pad_left, pad_right, pad_top, pad_bottom;

    // 缓存 RGA handle：input_mem_ 的 fd 从不变化，构造时 import 一次全生命周期复用
    rga_buffer_handle_t model_handle_{0};

    // 推理：接收 V4L2 DMABUF fd（NV12，img_width×img_height）
    // RGA 将其转换+缩放为 RGB888（model_width×model_height）后送 NPU
    int inference_image(int dmabuf_fd, detect_result_group_t &result_group);

    // 数据集测试：直接吃 OpenCV 读出的 BGR。按该图自己的宽高做 letterbox，不依赖摄像头 fd。
    // stride 为 0 时按紧密排列（bgr_w * 3）处理。
    int inference_bgr(const uint8_t *bgr, int bgr_w, int bgr_h, int stride,
                      detect_result_group_t &result_group);

private:
    // 三个 INT8 输出头反量化 + NMS，坐标映射回调用方给定的 letterbox 参数
    int decode_outputs(float scale_w, float scale_h, int pad_l, int pad_t,
                       detect_result_group_t &result_group);
};

#endif
