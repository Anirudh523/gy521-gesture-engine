file(GLOB_RECURSE EI_SOURCES
    "../edge-impulse-sdk/tflite-model/*.cpp"
    "../edge-impulse-sdk/edge-impulse-sdk/dsp/*.cpp"
    "../edge-impulse-sdk/edge-impulse-sdk/classifier/*.cpp"
    "../edge-impulse-sdk/edge-impulse-sdk/porting/*.cpp"
    "../edge-impulse-sdk/edge-impulse-sdk/tensorflow/lite/micro/*.cc"
    "../edge-impulse-sdk/edge-impulse-sdk/tensorflow/lite/micro/kernels/*.cc"
    "../edge-impulse-sdk/edge-impulse-sdk/tensorflow/lite/micro/memory_planner/*.cc"
    "../edge-impulse-sdk/edge-impulse-sdk/tensorflow/lite/core/api/*.cc"
    "../edge-impulse-sdk/edge-impulse-sdk/tensorflow/lite/kernels/internal/*.cc"
    "../edge-impulse-sdk/edge-impulse-sdk/tensorflow/lite/kernels/*.cc"
)

idf_component_register(
    SRCS
        "main.cpp"
        "IMUSensor.cpp"
        "ei_platform.cpp"
        ${EI_SOURCES}
    INCLUDE_DIRS
        "include"
        "../edge-impulse-sdk"
        "../edge-impulse-sdk/edge-impulse-sdk"
        "../edge-impulse-sdk/edge-impulse-sdk/third_party/flatbuffers/include"
        "../edge-impulse-sdk/edge-impulse-sdk/third_party/gemmlowp"
        "../edge-impulse-sdk/edge-impulse-sdk/third_party/ruy"
        "../edge-impulse-sdk/edge-impulse-sdk/tensorflow"
        "../edge-impulse-sdk/edge-impulse-sdk/tensorflow/lite/micro"
        "../edge-impulse-sdk/edge-impulse-sdk/porting/espressif/esp-dsp/modules/fft/include"
        "../edge-impulse-sdk/edge-impulse-sdk/porting/espressif/esp-dsp/modules/common/include"
)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-missing-field-initializers -Wno-maybe-uninitialized -Wno-error)

set_source_files_properties(
    ${CMAKE_CURRENT_SOURCE_DIR}/../edge-impulse-sdk/tflite-model/tflite_learn_1049225_4_compiled.cpp
    PROPERTIES COMPILE_FLAGS "-Wno-missing-field-initializers"
)
