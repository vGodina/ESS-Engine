#include "Convolution.h"
#include "AudioFFT.h"

namespace ESS
{
	struct Spectrum
	{
		std::vector<float> RealPart;
		std::vector<float> ImagPart;
	};

	Spectrum ToSpectrum(const Signal& SourceSignal, int FFTSize)
	{
		auto ZeroPaddedSignal = SourceSignal;
		ZeroPaddedSignal.resize(FFTSize, 0.0f);
		audiofft::AudioFFT Processed_Spectrum;
		Processed_Spectrum.init(FFTSize);

		int SpectrumSize = FFTSize / 2 + 1;
		std::vector<float> RealPart(SpectrumSize);
		std::vector<float> ImagPart(SpectrumSize);

		Processed_Spectrum.fft(ZeroPaddedSignal.data(), RealPart.data(), ImagPart.data());
		return Spectrum{ RealPart, ImagPart };
	}

	void MultiplySpectrums(const Spectrum& First, const Spectrum& Second, Spectrum& Output)
	{
		for (int i = 0; i < Output.RealPart.size(); ++i)
		{
			Output.RealPart[i] = First.RealPart[i] * Second.RealPart[i] - First.ImagPart[i] * Second.ImagPart[i];
			Output.ImagPart[i] = First.ImagPart[i] * Second.RealPart[i] + First.RealPart[i] * Second.ImagPart[i];
		}
	}

	void ToWaveform(Signal& DestSignal, const Spectrum& SourceSpectrum)
	{
		int FFTSize = 2* (SourceSpectrum.RealPart.size() - 1);
		DestSignal.resize(FFTSize);
		audiofft::AudioFFT Spectrum;
		Spectrum.init(FFTSize);
		Spectrum.ifft(DestSignal.data(), SourceSpectrum.RealPart.data(), SourceSpectrum.ImagPart.data());
	}

	int OutputSignalLength(const Signal& FirstSignal, const Signal&  SecondSignal)
	{
		const int ConvolutionLength = FirstSignal.size() + SecondSignal.size() + 1;
		int Result = 1;
		while (Result < ConvolutionLength)
			Result *= 2;
		return Result;
	}

	void Convolution(const Signal& FirstInput, const Signal& SecondInput, Signal& OutputSignal)
	{
		
		int ZeroPaddedLength = OutputSignalLength(FirstInput, SecondInput);
		const int SpectrumLength = ZeroPaddedLength / 2 + 1;

		Spectrum Output{ std::vector<float>(SpectrumLength), std::vector<float>(SpectrumLength) };

		MultiplySpectrums(	ToSpectrum(FirstInput, ZeroPaddedLength),
							ToSpectrum(SecondInput, ZeroPaddedLength),
							Output);

		ToWaveform(OutputSignal, Output);
	}
}
