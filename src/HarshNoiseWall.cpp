#include "Yggdrasil.hpp"


struct HarshNoiseWall : Module {
	enum ParamIds {
		PITCH_PARAM,
		HARSH_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		PITCH_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		SINE_OUTPUT,
		HNW_OUTPUT,
		NUM_OUTPUTS
	};

	float phase = 0.0;
	double sa;
	srand();
	
	HarshNoiseWall() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
	void step() override;

	// For more advanced Module features, read Rack's engine.hpp header file
	// - toJson, fromJson: serialization of internal data
	// - onSampleRateChange: event triggered by a change of sample rate
	// - reset, randomize: implements special behavior when user clicks these from the context menu
};


void HarshNoiseWall::step() {
	// Implement a simple sine oscillator
	float deltaTime = 1.0 / engineGetSampleRate();

	// Compute the frequency from the pitch parameter and input
	float pitch = params[PITCH_PARAM].value;
	pitch += inputs[PITCH_INPUT].value;
	pitch = clampf(pitch, -4.0, 4.0);
	float freq = 440.0 * powf(2.0, pitch);

	// Accumulate the phase
	phase += freq * deltaTime;
	if (phase >= 1.0)
		phase -= 1.0;

	// Compute the sine output
	float sine = sinf(2 * M_PI * phase);
	float random = randomNormal();
	outputs[SINE_OUTPUT].value = 5.0 * sine * random;
	
	int harshness = static_cast<int>(std::round(params[HARSH_PARAM].value));
	
	
	
	//HNW 
	//double ss[] = new double[harshness];
	//int r = rand();
	//printf("%d\n", r);
}


HarshNoiseWallWidget::HarshNoiseWallWidget() {
	HarshNoiseWall *module = new HarshNoiseWall();
	setModule(module);
	box.size = Vec(6 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/MyModule.svg")));
		addChild(panel);
	}

	addChild(createScrew<ScrewSilver>(Vec(15, 0)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 30, 0)));
	addChild(createScrew<ScrewSilver>(Vec(15, 365)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 30, 365)));

	addParam(createParam<Davies1900hBlackKnob>(Vec(8, 87), module, HarshNoiseWall::PITCH_PARAM, -3.0, 3.0, 0.0));
	
	addParam(createParam<Davies1900hBlackKnob>(Vec(48, 87), module, HarshNoiseWall::HARSH_PARAM, 1, 255.0, 0.0));

	addInput(createInput<PJ301MPort>(Vec(33, 186), module, HarshNoiseWall::PITCH_INPUT));

	addOutput(createOutput<PJ301MPort>(Vec(13, 275), module, HarshNoiseWall::SINE_OUTPUT));
	
	addOutput(createOutput<PJ301MPort>(Vec(53, 275), module, HarshNoiseWall::HNW_OUTPUT));
}
