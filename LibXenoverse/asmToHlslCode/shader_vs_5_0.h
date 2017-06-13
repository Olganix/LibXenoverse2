
#ifndef LIBXENOVERSE_shader_vs_5_0_H_INCLUDED
#define LIBXENOVERSE_shader_vs_5_0_H_INCLUDED

#include "LibXenoverse.h"

namespace LibXenoverse {

// this class try to convert automatically asm shader v5.0, into hsls shader in v3.0. 
// this is a draft code, same if it's used to genrerate shaders for Xenoviewer from Dbxv2's shaders.
// not all instructions are here. there is trouble with bit-wise instructions, and all samplers have the same use (not the case in ams).

class Shader_vs_5_0
{
public:
	enum ShaderInstruction
	{
		SI_unknow = 0,
		SI_dcl_globalFlags,
		SI_dcl_constantbuffer,
		SI_dcl_input,
		SI_dcl_output_siv,
		SI_dcl_output,
		SI_dcl_temps,
		SI_if_nz,
		SI_imul,
		SI_mul,
		SI_add,
		SI_mov,
		SI_dp4,
		SI_dp3,
		SI_sqrt,
		SI_max,
		SI_else,
		SI_rsq,
		SI_endif,
		SI_ret,
	};

	enum ShaderInstructionChannel
	{
		SIC_x = 1,
		SIC_y = 2,
		SIC_z = 4,
		SIC_w = 8,
	};

	enum ShaderDeclarationPart								//differents parts of declaration, sequential.
	{
		SDP_Unkonw = 0,
		SDP_BufferDefinitions,
		SDP_ResourceBindings,
		SDP_InputSignature,
		SDP_OutputSignature,
	};

	struct ShaderOperation
	{
		ShaderInstruction mInstruction;
		bool mIs_sat;
		size_t mListChannel;
		
		ShaderOperation(void)
		{
			mInstruction = SI_unknow;
			mIs_sat = false;
			mListChannel = 0;
		}

		void parse(std::string str) {}
	};
	


	struct ShaderRegistrer
	{
		std::string mName;
		size_t mListChannel;

		std::string mInputName;						//in case of Input signature
		size_t mNbInputDeclaration;					//we could have only on variable, fill with many TextureCoord apparently. So, we will add duplicate on this information to deal with that correctly.
		bool mOnlyUseNumberOfChannelForDefineType;	//in case of duplicate, the type will not be good. (hyp: on the first textureCoord, it use the first channels).

		ShaderRegistrer(std::string name)
		{
			mName = name;
			mListChannel = 0;
			mInputName = "";

			mNbInputDeclaration = 1;
			mOnlyUseNumberOfChannelForDefineType = false;
		}

		void	addChannels(std::string str)
		{
			if (mListChannel == 15)				//if we allready have all channels, we could skip
				return;

			size_t nbChannels = str.length();
			for (size_t i = 0; i < nbChannels; i++)
				addChannel(str.substr(i,1));
		}

		void	addChannel(std::string str)
		{
			if (mListChannel == 15)				//if we allready have all channels, we could skip
				return;

			if ((str == "x") || (str == "r"))
				mListChannel = mListChannel | SIC_x;
			else if ((str == "y") || (str == "g"))
				mListChannel = mListChannel | SIC_y;
			else if ((str == "z") || (str == "b"))
				mListChannel = mListChannel | SIC_z;
			else if ((str == "w") || (str == "a"))
				mListChannel = mListChannel | SIC_w;
		}

		string getChannelOrdered(void)
		{
			string str = "";
			if (mListChannel & SIC_x)
				str += "x";
			if (mListChannel & SIC_y)
				str += "y";
			if (mListChannel & SIC_z)
				str += "z";
			if (mListChannel & SIC_w)
				str += "w";

			return str;
		}

		std::string	getFinalDeclaration(bool isInput = false, bool isOuputRegistrer = false)
		{
			string name = mName;
			
			if((isOuputRegistrer)&&(mNbInputDeclaration!=1))
				name += "_0";

			if (mOnlyUseNumberOfChannelForDefineType)
			{
				size_t nbChannel = ((mListChannel & SIC_x) ? 1 : 0) + ((mListChannel & SIC_y) ? 1 : 0) + ((mListChannel & SIC_z) ? 1 : 0) + ((mListChannel & SIC_w) ? 1 : 0);
				return "\tfloat" + ((nbChannel>1) ? std::to_string(nbChannel) : "") + " " + name + ((!isInput) ? (" = " + ((nbChannel>1) ? ("float"+ std::to_string(nbChannel) + "(" + fillArg(nbChannel, "0") + ")") : "0.0") +";\n") : ("\t\t:\t" + mInputName));
			}
						
			if (mListChannel & SIC_w)
				return "\tfloat4 " + name + ((!isInput) ? (" = float4(0,0,0,0);\n") : ("\t\t:\t"+ mInputName));
			if (mListChannel & SIC_z)
				return "\tfloat3 " + name + ((!isInput) ? (" = float3(0,0,0);\n") : ("\t\t:\t"+ mInputName));
			if (mListChannel & SIC_y)
				return "\tfloat2 " + name + ((!isInput) ? (" = float2(0,0);\n") : ("\t\t:\t" + mInputName));
			if ((mListChannel & SIC_x) && (!isInput))
				return "\tfloat2 " + name + ((!isInput) ? (" = float2(0,0);\n") : ("\t\t:\t" + mInputName));
			else
				return "\tfloat " + name + ((!isInput) ? " = 0.0;\n" : ("\t\t:\t" + mInputName));
		}

		std::string fillArg(size_t nbChannel, std::string strToFill)
		{
			std::string str = "";
			for (size_t i = 0; i < nbChannel; i++)
				str += ((str.length() != 0) ? "," : "") + strToFill;
			return str;
		}
	};


	struct ShaderCBuffer_Var
	{
		std::string mName;
		std::string mType;						// "float4x4", "float4", float3 , ...
		size_t mPositionInBuffer;
		size_t mSize;


		bool mIsArray;
		size_t mNbArrayElements;

		ShaderCBuffer_Var(std::string name, std::string s_type, size_t positionInBuffer, size_t size)
		{
			mName = name;
			mType = s_type;
			mPositionInBuffer = positionInBuffer;
			mSize = size;
			
			if (mType.substr(0, 6) == "float4")				//acces on a "float4*" buffer is differente from bool
			{
				mPositionInBuffer /= 4;
				mSize /= 4;
			}

			mIsArray = false;
			mNbArrayElements = 0;

			//case of bone's matrix : float4x3 g_mMatrixPalette_VS[24]
			if(mName.find('[')!=std::string::npos)
			{ 
				mIsArray = true;
				std::vector<std::string> sv = split(mName.substr(0, mName.length()-1), '[');
				mName = sv.at(0);

				std::string::size_type sz;
				mNbArrayElements = std::stoi(sv.at(1).c_str(), &sz);
			}
		}
	};

	struct ShaderCBuffer
	{
		std::string mName;
		std::vector<ShaderCBuffer_Var> mListShaderCBuffer_Var;
		std::string mCodeName;						//"cbX"

		ShaderCBuffer(std::string name)
		{
			mName = name;
			mCodeName = "";
		}


		std::string	getArgumentString(std::string inside, std::string onRight)
		{
			std::string onLeft = mCodeName;

			if (isNumber(inside))
			{
				std::string::size_type sz;
				size_t indexFloat = std::stoi(inside.c_str(), &sz) * 4;

				bool isfound = false;
				size_t nbVar = mListShaderCBuffer_Var.size();
				for (size_t i = 0; i < nbVar; i++)
				{
					ShaderCBuffer_Var &var = mListShaderCBuffer_Var.at(i);
					if ((indexFloat >= var.mPositionInBuffer) && (indexFloat < var.mPositionInBuffer + var.mSize))
					{
						onLeft = var.mName;
						indexFloat -= var.mPositionInBuffer;
						indexFloat /= 4;
						inside = std::to_string(indexFloat);

						if ((var.mType == "float4") || (var.mType == "float3") || (var.mType == "float2") )		//if is not a array, directly a variable, like a float4.
						{
							return onLeft + onRight;

						}else if ((var.mType == "bool") || (var.mType == "float") || (var.mType == "int") ) {

							return onLeft;					//for simple values , no need right channel informations

						}else if ( (var.mType == "float4x4") || (var.mType == "float4x3") ) {


							size_t col = indexFloat;


							string channel = onRight.substr(1);					//remove the '.'
							
							onRight = ".";
							string currentChannel = "";
							size_t nbChar = channel.length();
							for (size_t j = 0; j < nbChar; j++)
							{
								currentChannel = channel.at(j);
								onRight += string("_m") + ((currentChannel == "x") ? "0" : ((currentChannel == "y") ? "1" : ((currentChannel == "z") ? "2" : "3"))) + std::to_string(col);
							}

							return onLeft + onRight;
						}


						isfound = true;
						break;
					}
				}

				if (!isfound)
				{
					LOG_DEBUG("Problem with %s.\n", (onLeft + "[" + inside + "]" + onRight).c_str());
				}

			}else if (mListShaderCBuffer_Var.size() == 1) {

				onLeft = mListShaderCBuffer_Var.at(0).mName;

				//test to deal with  float4x4 ou float4x3 // todo do better
				if ((mListShaderCBuffer_Var.at(0).mType == "float4x4") || (mListShaderCBuffer_Var.at(0).mType == "float4x3"))
				{
					std::vector<std::string> sv = split(inside, '+');
					if (sv.size() == 2)												//hyp: the +1 ou +2 ou +3 est here to choose the column. after, what is before select the bone.
					{

						string channel = onRight.substr(1);					//remove the '.'

						onRight = ".";
						string currentChannel = "";
						size_t nbChar = channel.length();
						for (size_t j = 0; j < nbChar; j++)
						{
							currentChannel = channel.at(j);
							onRight += string("_m") + ((currentChannel=="x") ? "0" : ((currentChannel == "y") ? "1" : ((currentChannel == "z") ? "2" : "3") ) ) + sv.at(1);
						}

						return onLeft +"["+ sv.at(0) + "]"+ onRight;
					}
				}

			}else {

				LOG_DEBUG("Problem with %s.\n", (onLeft + "[" + inside + "]" + onRight).c_str() );
				//getchar();
			}

			return onLeft + "[" + inside + "]" + onRight;
		}

	};








	struct Shader_AsmArg
	{
		string mArg;
		ShaderCBuffer_Var* mVar;

		//have to analyse to have this informations :
		bool mIsAnalysed;
		string mName;
		string mType;
		bool mIsNumber;
		bool mIsNegative;
		bool mIsAbsolute;
		bool mIsChannelsSpecified;
		std::vector<ShaderInstructionChannel> mListChannel;
		std::vector<string> mListChannelNameSpeclalArrayCase;


		Shader_AsmArg(string arg, ShaderCBuffer_Var* var = 0)
		{
			mArg = arg;
			mVar = var;

			mIsAnalysed = false;
			mName = "";
			mType = "";
			mIsNumber = false;
 			mIsChannelsSpecified = false;
			mIsNegative = false;
			mIsAbsolute = false;

			analyse();
		}

		void analyse(void)
		{
			//try to have the type
			//try to understand the channels
			
			string arg = mArg;
			size_t nbChar = arg.length();

			mName = arg;
			mIsAnalysed = true;
			if(nbChar == 0)
				return;



			mIsNumber = isNumber(arg);
			if (mIsNumber)
				return;

			//cas -aa.xy
			mIsNegative = (arg.substr(0, 1) == "-");
			if(mIsNegative)
				arg = arg.substr(1);
			
			if (nbChar > 5)
			{
				//case abs(xxxxxx)
				mIsAbsolute = (arg.substr(0, 4) == "abs(");
				if (mIsAbsolute)
				{
					arg = arg.substr(4, arg.length() - 4 - 1);
					mName = arg;
				}
			}

			if (nbChar > 6)
			{
				//case intX(,,,)
				bool isInt = (arg.substr(0, 3) == "int");
				if (isInt)
				{
					mName = arg;
					mType = arg.substr(0, 4);
					return;
				}
			}

			if (nbChar > 8)
			{
				//case floatX(,,,)
				//case floatX( 1.0 , 2.0 , 2.3 , 3.4 )
				bool isFloat = (arg.substr(0, 5) == "float");
				if (isFloat)
				{
					mName = arg;
					mType = arg.substr(0, 6);
					return;
				}
			}
			

			//case aa.xyzw
			//case aa._m00_m01.m10
			//case aa
			size_t pos = arg.find_last_of('.');
			if (pos != string::npos)
			{
				mIsChannelsSpecified = true;

				mName = arg.substr(0, pos);
				string channel = arg.substr(pos + 1);
				size_t nbChannel = channel.size();


				if ((nbChannel > 2) && (channel.substr(0, 2) == "_m"))		//case of matrix
				{
					string str = channel;
					channel = "";
					size_t inc = 0;
					while ((str.size() >= 4) && (str.substr(0, 2) == "_m"))
					{
						channel += getChannelIndex(inc);
						mListChannelNameSpeclalArrayCase.push_back(str.substr(0, 4));
						inc++;
						str = str.substr(4);
					}
					nbChannel = channel.size();
				}
				

				if (nbChannel > 4)
					nbChannel = 4;

				for (size_t i = 0; i < nbChannel; i++)
					mListChannel.push_back( getChannel( channel.substr(i,1) ) );
			}

			if (mVar)
			{
				mType = mVar->mType;
				if ((mType == "float") || (mType == "int") || (mType == "bool"))
				{
					mListChannel.clear();
					mIsChannelsSpecified = true;
				}
			}
		}


		ShaderInstructionChannel getChannel(string value)
		{
			if ((value == "x") || (value == "r"))
				return SIC_x;
			if ((value == "y") || (value == "g"))
				return SIC_y;
			if ((value == "z") || (value == "b"))
				return SIC_z;
			if ((value == "w") || (value == "a"))
				return SIC_w;
			
			return SIC_x;
		}

		string getChannelName(ShaderInstructionChannel value)
		{
			switch (value)
			{
			case SIC_x: return "x"; break;
			case SIC_y: return "y"; break;
			case SIC_z: return "z"; break;
			case SIC_w: return "w"; break;
			}
			return "";
		}


		string getChannelIndex(size_t index)
		{
			if (index < mListChannelNameSpeclalArrayCase.size())		//special case matrix
				return mListChannelNameSpeclalArrayCase.at(index);

			if (index < mListChannel.size())
				return getChannelName(mListChannel.at(index));

			return getChannelName((ShaderInstructionChannel)( (int)pow(2,index) ));
		}

		string getAllChannelString(size_t limitNbChannels = (size_t)-1)
		{
			size_t nbChannel = mListChannel.size();
			if (limitNbChannels < nbChannel)
				nbChannel = limitNbChannels;

			string str = "";
			for (size_t i = 0; i < nbChannel; i++)
				str += getChannelIndex(i);

			if (limitNbChannels != (size_t)-1)
			{
				for (size_t i = nbChannel; i < limitNbChannels; i++)		//force to complete channels
					str += getChannelIndex(i);
			}

			return str;
		}

		

		bool haveNoChannel(void)
		{
			return ((mIsChannelsSpecified) && (mListChannel.size() == 0)) ;
		}



		string getArg(size_t limitNbChannels = (size_t)-1)
		{
			if (!mIsAnalysed)
				analyse();

			if (mIsNumber)
				return mName;

			if(haveNoChannel())
				return string((mIsAbsolute) ? "abs(" : "") + ((mIsNegative) ? "-" : "") + mName + ((mIsAbsolute) ? ")" : "");

			return string((mIsAbsolute) ? "abs(" : "") + ((mIsNegative) ? "-" : "") + mName +"."+ getAllChannelString(limitNbChannels) + ((mIsAbsolute) ? ")" : "");
		}

		string getArg_ForChannel(size_t indexChannel)
		{
			if (!mIsAnalysed)
				analyse();

			if (mIsNumber)
				return mName;

			if (haveNoChannel())
				return string((mIsAbsolute) ? "abs(" : "") + ((mIsNegative) ? "-" : "") + mName + ((mIsAbsolute) ? ")" : "");

			return string((mIsAbsolute) ? "abs(" : "") + ((mIsNegative) ? "-" : "") + mName + "." + getChannelIndex(indexChannel) + ((mIsAbsolute) ? ")" : "");
		}

	};





	struct ShaderSampler
	{
		std::string mName;
		std::string mCodeName;						// "sX"
		std::string mType;							// "2d", ...

		ShaderSampler(std::string name, std::string slot, std::string s_type)
		{
			mName = name;
			mCodeName = "s"+ slot;
			mType = s_type;
		}
	};

	std::string mName;
	std::string mHlslCode;
	std::vector<ShaderOperation> mListShaderOperation;
	std::vector<ShaderRegistrer> mListShaderRegistrer;						//temporary variables in shader.
	std::vector<ShaderRegistrer> mListShaderRegistrer_VertexInput;			//to deal with Input signatures
	std::vector<ShaderRegistrer> mListShaderRegistrer_Ouput;				//same for Ouput.
	std::vector<ShaderCBuffer> mListShaderCBuffer;							//here, we declare cbuffer.
	std::vector<ShaderSampler> mListShaderSampler;							//herer, texture's Samplers.

	ShaderDeclarationPart mShaderDeclarationPart;
	

public:
	Shader_vs_5_0(string name) { mName = name; mHlslCode = ""; }
	~Shader_vs_5_0(void) {}

	ShaderCBuffer_Var* getShaderCBuffer_Var(const string &name);
	string	rectifyChannels(const string &ouputArg);

	void	parseAsm(std::string asm_str);
	std::string	writeHlsl(void);
};


}

#endif