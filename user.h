#pragma once
namespace ESS
{
	// user functions
	/*
	Entities:
	There are Record and Play Audio Devices and corresponding streams.
	There is Graphic window that shows waveform of played Ref signal.
	Also
	Ref,Processed and IRs are 
	Generate(Ref)
	There are controls: Start {Playback(Ref) + Record(Processed)}
	*/
	// set up devices or sample rate
	void SetHardware();

	void EnterSignalLength();

	void ShowRefSignal(); // optional

	void SaveRefSignal();

	//calls ProcessSignal
	void StartMeasuring() {};
	
	//optional
	void ShowProcessedSignal();

	void SaveProcessed();

	void CalculateIRs();

	void ShowIRs();

	void SaveIRs();

	void ShowIRSpectrum();
} //namespace ESS
