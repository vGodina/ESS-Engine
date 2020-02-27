#include <algorithm>
#include <string>
#include "audiofile.h"
#include "AudioFiles.h"

namespace AudioFiles
{
	void Attenuate(Signal& SourceSignal, const double Attenuator)
	{
		for (auto& Element : SourceSignal) 
		{
			Element /= Attenuator;
		}
	}

	float FindAbsMax(const Signal& SourceSignal)
	{
		const auto MinMax = std::minmax_element(SourceSignal.begin(), SourceSignal.end());
		return (*MinMax.second > abs(*MinMax.first)) ? *MinMax.second : abs(*MinMax.first);
	}

	bool SaveToFile(const std::string& FilePath, const Signal& SourceSignal, const int BitDepth, const int SampleRate)
	{
		AudioFile<float>::AudioBuffer buffer{ SourceSignal };
		AudioFile<float> audioFile;
		audioFile.setBitDepth(BitDepth);
		audioFile.setSampleRate(SampleRate);
		audioFile.setAudioBuffer(buffer);

		return audioFile.save(FilePath);
	}

	bool SaveToFile(const std::string& FilePath, const Signal& SourceSignal, const int BitDepth, const int SampleRate, const double Attenuator)
	{
		const auto AttenuateFactor = FindAbsMax(SourceSignal) / Attenuator;
		Signal NormalizedSignal = SourceSignal;
		Attenuate(NormalizedSignal, AttenuateFactor);
		const auto AttenuateDecibels = -20 * log10(AttenuateFactor);
		std::size_t found = FilePath.find(".wav");
		return SaveToFile(FilePath.substr(0,found) + "_" + std::to_string(AttenuateDecibels) + "dB.wav", NormalizedSignal, BitDepth, SampleRate);
	}

	Signal ImportFromFile(const std::string& FilePath)
	{
		AudioFile<float> audioFile;
		audioFile.load(FilePath);
		return audioFile.samples[0];
	}
}
