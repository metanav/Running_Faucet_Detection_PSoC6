#ifndef __MODEL_JSON_H__
#define __MODEL_JSON_H__

const char recognition_model_string_json[] = {"{\"NumModels\":1,\"ModelIndexes\":{\"0\":\"PIPELINE_1_RANK_0\"},\"ModelDescriptions\":[{\"Name\":\"PIPELINE_1_RANK_0\",\"ClassMaps\":{\"1\":\"faucet\",\"2\":\"noise\",\"0\":\"Unknown\"},\"ModelType\":\"TensorFlowLiteforMicrocontrollers\",\"FeatureFunctions\":[\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\",\"MFCC\"]}]}"};

int recognition_model_string_json_len = sizeof(recognition_model_string_json);

#endif /* __MODEL_JSON_H__ */
