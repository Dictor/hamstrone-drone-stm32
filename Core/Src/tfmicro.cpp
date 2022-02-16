#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"
#include "model_data.h"
#include "tfmicro.h"

//namespace {
	const int tensor_arena_size = 2 * 1024;
	uint8_t tensor_arena[tensor_arena_size];
	tflite::MicroInterpreter *interpreter;
	TfLiteTensor *input, *output;
	tflite::MicroMutableOpResolver<3> micro_op_resolver;
//}

extern "C" {
	int initTF()
	{
		const tflite::Model* model = ::tflite::GetModel(policy_tflite);
		if (model->version() != TFLITE_SCHEMA_VERSION)
		{
			return -1;
		}
		tfdebug("tf: get model ok");

		micro_op_resolver.AddFullyConnected();
		micro_op_resolver.AddSoftmax();
		micro_op_resolver.AddRelu();
		tfdebug("tf: op resolver ok");

		static tflite::MicroInterpreter currentInterpreter(model, micro_op_resolver, tensor_arena, tensor_arena_size, nullptr);
		tfdebug("tf: interpreter ok");

		interpreter = &currentInterpreter;
		interpreter->AllocateTensors();
		tfdebug("tf: allocate ok");
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
