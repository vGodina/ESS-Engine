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
	const int OctavesCount = 11;
	const int LengthParam = 61;
	const double Attenuate = 0.9995;
	// IR parameters
	const int HarmonicsCount = 20;
	const double IRLength = 1.0;
	const double PreFrame = 0.03;
	const double HarmonicsLength = 0.1;
	// file path parameters
	const std::string FolderPath = "C:/D/";
	const std::string RefSignalPath = FolderPath + "ExportedRef.wav";
	const std::string IRSignalPath = FolderPath + "RawIR.wav";
	const std::string IRPath = FolderPath + "IR";
	const std::string ProcessedSignalPath = FolderPath + "Processed.wav";

	ESS::EssEngine Engine;
	Engine.SetSignalParameters(SampleRate, OctavesCount, LengthParam);
	Engine.GenerateReferenceSignal();
	Engine.ImportProcessedSignal(ESS::ImportFromFile(ProcessedSignalPath));
	Engine.CalculateRawIR();
	Engine.SeparateIRs(HarmonicsCount, IRLength, HarmonicsLength, PreFrame);

	ESS::SaveToFile(RefSignalPath, Engine.GetReferenceSignal(), BitDepth, SampleRate, Attenuate);
	ESS::SaveToFile(IRSignalPath, Engine.GetRawIR(), BitDepth, SampleRate, Attenuate);

	int i = 0;
	for (const auto& IR : Engine.GetIRArray())
	{
		++i;
		ESS::SaveToFile(IRPath + std::to_string(i) + ".wav", IR, BitDepth, SampleRate, Attenuate);
	}

	return 0;
}
