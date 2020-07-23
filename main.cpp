#include <iostream>
#include <string>

#include "EssEngine.h"
#include "AudioFiles.h"
#include "TestUtil.h"
#include "user.h"


int main()
{
	//ESS signal parameters
	const int SampleRate = 48000;
	const int BitDepth = 24;
	const double LowerFrequency = 23;

	const double SignalDurationInSec = 12;
	const double Attenuate = 0.9995;
	// IR parameters
	const int HarmonicsCount = 5;
	const double IRLength = 1.5;
	const double PreFrame = 0.03;
	const double HarmonicsLength = 0.1;
	// file path parameters
	const std::string FolderPath = "C:/D/";
	const std::string RefSignalPath = FolderPath + "ExportedRef.wav";
	const std::string IRSignalPath = FolderPath + "RawIR.wav";
	const std::string IRPath = FolderPath + "IR";
	const std::string ProcessedSignalPath = FolderPath + "Processed.wav";

	ESS::EssEngine Engine;
	Engine.SetSampleRate(SampleRate);
	const int OctavesCount = 11;// Engine.EstimateOctaveCount(LowerFrequency);
	const int LengthCoefficient = 23;// Engine.EstimateLengthCoeff(SignalDurationInSec);
	Engine.SetOctavesCount(OctavesCount);
	Engine.SetLengthCoefficient(LengthCoefficient);
	Engine.GenerateReferenceSignal();
	//AudioFiles::SaveToFile(RefSignalPath, Engine.GetReferenceSignal(), BitDepth, SampleRate, Attenuate);
	Engine.ImportProcessedSignal(AudioFiles::ImportFromFile(ProcessedSignalPath));
	Engine.CalculateRawIR();
	Engine.SeparateIRs(HarmonicsCount, IRLength, HarmonicsLength, PreFrame);

	AudioFiles::SaveToFile(IRSignalPath, Engine.GetRawIR(), BitDepth, SampleRate, Attenuate);

	int i = 0;
	for (const auto& IR : Engine.GetIRArray())
	{
		++i;
		AudioFiles::SaveToFile(IRPath + std::to_string(i) + ".wav", IR, BitDepth, SampleRate, Attenuate);
	}

	return 0;
}
