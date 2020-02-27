#include <algorithm>
#include <iostream>
#include "EssEngine.h"
#include "Convolution.h"

namespace ESS
{
	double RMS(const Signal& SourceSignal)
	{
		double Sum{ 0.0 };
		for (auto Sample : SourceSignal) {
			Sum += Sample * Sample;
		}
		return sqrt(Sum / SourceSignal.size());
	}

	Signal GetEnvelope(const Signal& SourceSignal, double WindowInSeconds, int SampleRate)
	{
		int WindowInSamples = WindowInSeconds * SampleRate;
		auto Begin = SourceSignal.begin();
		Signal Envelope;
		Envelope.resize(SourceSignal.size() / WindowInSamples);

		for (int i = 0; i < Envelope.size(); ++i)
		{
			Envelope[i] = RMS(Signal{ Begin, Begin + WindowInSamples });
			Begin += WindowInSamples;
		}
		return Envelope;
	}

	ptrdiff_t FindIndexAbsMax(const Signal& SourceSignal)
	{
		const auto MinMax = std::minmax_element(SourceSignal.begin(), SourceSignal.end());
		return (*MinMax.second > abs(*MinMax.first)) ?
			MinMax.second - SourceSignal.begin() : MinMax.first - SourceSignal.begin();
	}

	std::vector<int> FindIRPeakIndexes(const Signal& SourceSignal, int HarmonicsCount, const int OctaveLength)
	{
		int MainIndex = FindIndexAbsMax(SourceSignal);
		std::vector<int> Peaks;
		for (int i = 0; i < HarmonicsCount; ++i) {
			Peaks.emplace_back(MainIndex - log2(i + 1) * OctaveLength);
		}
		return Peaks;
	}

	double FindNoiseFloor(const Signal& SourceSignal) { return 0.0; }

	Signal FindImpulseAroundPeakIndex(const Signal& SourceSignal, const int PeakIndex, const double PreFrame, const double Length, const int SampleRate)
	{
		//naive implementation
		Signal Result;
		ptrdiff_t LeftPause = PreFrame * SampleRate;
		ptrdiff_t ImpulseLength = Length * SampleRate;
		auto Begin = SourceSignal.begin() + PeakIndex - LeftPause;
		auto End = SourceSignal.begin() + PeakIndex + ImpulseLength;

		return Signal(Begin, End);
	}

	int EssEngine::EstimateLengthCoeff(double LengthInSec)
	{
		return 0;
	}

	int EssEngine::EstimateOctaveCount(double LowerFrequency)
	{
		return round(log2(0.5 * SampleRate / LowerFrequency));
	}

	void EssEngine::SetSampleRate(const int InSampleRate)
	{
		SampleRate = InSampleRate;
	}

	void EssEngine::SetSignalParameters(const int InOctavesCount, const int InLengthCoefficient)
	{
		OctavesCount = InOctavesCount;
		LengthCoefficient = InLengthCoefficient;
	}

	void EssEngine::SetKirkebyFilter()
	{

	}

	void EssEngine::GenerateReferenceSignal()
	{
		double UnroundedSignalLength = OctavesCount * LengthCoefficient * log(2.0) * pow(2.0, OctavesCount + 1);
		int RoundedSignalLength = static_cast<int>(round(UnroundedSignalLength));
		ReferenceSignal.resize(RoundedSignalLength);

		constexpr double PI = 3.141592653589793116;

		for (int i = 0; i < RoundedSignalLength; ++i)
		{
			ReferenceSignal[i] = sin(2.0 * PI * LengthCoefficient * pow(2.0, OctavesCount * i / static_cast<double>(RoundedSignalLength)));
		}
	}

	Signal EssEngine::GenerateInverseSignal(const Signal& RefSignal)
	{
		auto InverseSignal = RefSignal;
		std::reverse(InverseSignal.begin(), InverseSignal.end());
		double EnergyCoeff = OctavesCount * log(2.0) / (1.0 - pow(2.0, -OctavesCount));
		double PowCoeff = -OctavesCount / static_cast<double>(InverseSignal.size());
		for (int i = 0; i < InverseSignal.size(); ++i)
		{
			InverseSignal[i] *= EnergyCoeff * pow(2.0, PowCoeff * i);
		}

		return InverseSignal;
	}

	void EssEngine::CalculateRawIR()
	{
		Convolution(ProcessedSignal, GenerateInverseSignal(ReferenceSignal), RawIR);
	}

	void EssEngine::SeparateIRs(const int HarmonicsCount, const double IRMaxLength, const double HarmonicLength, const double PreFrame)
	{
		auto OctaveLength = ReferenceSignal.size() / OctavesCount;
		auto PeaksArray = FindIRPeakIndexes(RawIR, HarmonicsCount, OctaveLength);
		IRVector.emplace_back(FindImpulseAroundPeakIndex(RawIR, PeaksArray[0], PreFrame *50, IRMaxLength, SampleRate));
		for (int i = 1; i < PeaksArray.size(); ++i)
		{
			IRVector.emplace_back(FindImpulseAroundPeakIndex(RawIR, PeaksArray[i], PreFrame, HarmonicLength, SampleRate));
		}
	}

	void EssEngine::ImportProcessedSignal(const Signal& InProcessedSignal)
	{
		ProcessedSignal = InProcessedSignal;
	}

	void EssEngine::TrimProcessedSignalToRef()
	{
		// cross-corelate, find delay, cut begin if possible, cut end by known length
	}

	Signal EssEngine::GetReferenceSignal() const
	{
		return ReferenceSignal;
	}

	Signal EssEngine::GetRawIR() const
	{
		return RawIR;
	}

	std::vector<Signal> EssEngine::GetIRArray() const
	{
		return IRVector;
	}

} // namespace ESS
