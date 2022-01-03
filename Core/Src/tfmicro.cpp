#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

//namespace {
	tflite::Model *model;
	const int tensor_arena_size = 2 * 1024;
	uint8_t tensor_arena[tensor_arena_size];
	tflite::MicroInterpreter *interpreter;
	TfLiteTensor *input, *output;
	tflite::MicroMutableOpResolver<4> micro_op_resolver;
//}

extern "C" {
	int initTF()
	{
		//model = ::tflite::GetModel(g_model);
		if (model->version() != TFLITE_SCHEMA_VERSION)
		{
			return -1;
		}
		tflite::MicroInterpreter currentInterpreter(model, micro_op_resolver, tensor_arena, tensor_arena_size, nullptr);
		interpreter = &currentInterpreter;
		interpreter->AllocateTensors();
		input = interpreter->input(0);
		return 0;
	}

	int inferenceModel(uint8_t *inputValue, uint8_t inputSize, float *result)
	{
		memcpy(input->data.f, inputValue, inputSize);
		TfLiteStatus invoke_status = interpreter->Invoke();
		if (invoke_status != kTfLiteOk)
		{
			return -1;
		}
		output = interpreter->output(0);
		result = output->data.f;
		return 0;
	}
}
