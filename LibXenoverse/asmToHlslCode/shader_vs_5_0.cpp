

#include "Shader_vs_5_0.h"
#include "LibXenoverse.h"

namespace LibXenoverse
{


/*-------------------------------------------------------------------------------\
|                             parseAsm				                             |
\-------------------------------------------------------------------------------*/
void Shader_vs_5_0::parseAsm(std::string asm_str)
{
	string baseFilename = LibXenoverse::nameFromFilenameNoExtension(mName);
	
	
	mListShaderOperation.clear();
	mListShaderRegistrer.clear();
	mListShaderRegistrer_VertexInput.clear();
	std::string currentCBufferName = "";

	mShaderDeclarationPart = SDP_Unkonw;

	
	std::vector<std::string> sv = split(asm_str, '\n');

	//first version, we only read and trad directly.
	

	std::vector<std::string> sv2, sv3;
	std::vector<std::string> registreResult;
	string textCoordIndexInc_Input = "";
	string textCoordIndexInc_Output = "0";
	size_t targetIndexInc_Output = 0;

	std::string tabInc = "\t";
	mHlslCode = tabInc + "float tmp = 0;\n";
	bool isAPixelShader = false;

	size_t nbLines = sv.size();
	for (size_t i = 0; i < nbLines; i++)
	{
		std::string &line = sv.at(i);
		if (line == "")
			continue;


		
		if (line.substr(0, 2) == "//")
		{
			//gestion of Registre and I/O variable's declaration .

			if (line == "// Buffer Definitions: ")
			{
				mShaderDeclarationPart = SDP_BufferDefinitions;
				continue;
			}else if (line == "// Resource Bindings:"){
				mShaderDeclarationPart = SDP_ResourceBindings;
				continue;
			}else if (line == "// Input signature:") {
				mShaderDeclarationPart = SDP_InputSignature;
				continue;
			}else if (line == "// Output signature:") {
				mShaderDeclarationPart = SDP_OutputSignature;
				continue;
			}

			if ((mShaderDeclarationPart == SDP_Unkonw) || (line.size()==2) || (line.substr(0,8)=="// Name ") || (line.substr(0, 8) == "// -----") || (line.substr(0, 16) == "// Approximately") || (line.substr(0, 11) == "// no Input"))
				continue;
			


			if (mShaderDeclarationPart == SDP_BufferDefinitions)
			{
				if (line.substr(0, 10) == "// cbuffer")
				{
					currentCBufferName = line.substr(11);
					mListShaderCBuffer.push_back(ShaderCBuffer(currentCBufferName));
				
				}else if (line == "// {") {
					continue;

				}else if(line=="// }") {
					currentCBufferName = "";

				}else if(currentCBufferName!=""){				//if we don't finish the current cbuffer, we will get informations.

					string line_tmp = line.substr(5);

					size_t pos = line_tmp.find("/");
					if ((pos != string::npos) && (line_tmp.substr(pos + 1, 1) == "/") && (line_tmp.substr(pos - 1, 1) == ";"))
						line_tmp = line_tmp.substr(0, pos) +" "+ line_tmp.substr(pos);

					sv2 = split(line_tmp, ' ');
					for (size_t j = 0; j < sv2.size(); j++)
					{
						if (sv2.at(j) == "")
						{
							sv2.erase(sv2.begin() + j);
							j--;
						}
					}

					std::string::size_type sz;
					mListShaderCBuffer.back().mListShaderCBuffer_Var.push_back(ShaderCBuffer_Var(sv2.at(1).substr(0, sv2.at(1).length()-1), sv2.at(0), std::stoi(sv2.at(4).c_str(), &sz), std::stoi(sv2.at(6).c_str(), &sz)));
				}



			}else if (mShaderDeclarationPart == SDP_ResourceBindings){

				
				sv2 = split(line.substr(3), ' ');
				for (size_t j = 0; j < sv2.size(); j++)
				{
					if (sv2.at(j) == "")
					{
						sv2.erase(sv2.begin() + j);
						j--;
					}
				}

				std::string cbName = sv2.at(0);
				std::string cbtype = sv2.at(1);
				std::string cbSlot = sv2.at(4);
				
				if (cbtype == "cbuffer")
				{
					size_t nbBuffers = mListShaderCBuffer.size();
					for (size_t j = 0; j < nbBuffers; j++)
					{
						if (mListShaderCBuffer.at(j).mName == cbName)
						{
							mListShaderCBuffer.at(j).mCodeName = "cb" + cbSlot;
							break;
						}
					}

				}else if (cbtype == "texture") {

					mListShaderSampler.push_back(ShaderSampler(cbName, cbSlot, sv2.at(3)));
				}else if (cbtype == "sampler") {

					//Not need to use this , because there isn't differences with texture in Shader Model 3.0
				}else {
					LOG_DEBUG("cbtype unknow %s in line %s\n", cbtype.c_str(), line.c_str() );
				}



			}
			else if (mShaderDeclarationPart == SDP_InputSignature) {

				sv2 = split(line.substr(3), ' ');
				for (size_t j = 0; j < sv2.size(); j++)
				{
					if (sv2.at(j) == "")
					{
						sv2.erase(sv2.begin() + j);
						j--;
					}
				}


				bool isTextcoord = (sv2.at(0) == "TEXCOORD");
				bool isColor = (sv2.at(0) == "COLOR");

				if (sv2.at(0) == "SV_IsFrontFace")				// https://msdn.microsoft.com/en-us/library/windows/desktop/bb509647(v=vs.85).aspx
					sv2.at(0) = "VFACE";						//Direct3D 9 Shader Model 3.0.

				string registerName = "v" + sv2.at(3);

				size_t isfound = (size_t)-1;
				size_t nbinput = mListShaderRegistrer_VertexInput.size();
				for (size_t j = 0; j < nbinput; j++)
				{
					if (mListShaderRegistrer_VertexInput.at(j).mName == registerName)
					{
						isfound = j;
						break;
					}
				}

				if (isfound == (size_t)-1)
				{
					mListShaderRegistrer_VertexInput.push_back(ShaderRegistrer(registerName));
					mListShaderRegistrer_VertexInput.back().addChannels(sv2.at(2));
					mListShaderRegistrer_VertexInput.back().mInputName = sv2.at(0) + (((isTextcoord || isColor) && (sv2.at(1) != "0")) ? sv2.at(1) : "");
				}else {

					//we get the first declaration
					ShaderRegistrer &input = mListShaderRegistrer_VertexInput.at(isfound);
					input.addChannels(sv2.at(2));							// we add channels to complete the unique variable.

					//we will build the duplicate by rename to avoid get him in future search.
					ShaderRegistrer duplicated(registerName +"_"+ std::to_string(input.mNbInputDeclaration));
					duplicated.addChannels(sv2.at(2));
					textCoordIndexInc_Input = sv2.at(1);
					duplicated.mInputName = sv2.at(0) + (((isTextcoord || isColor) && (sv2.at(1) != "0")) ? sv2.at(1) : "");
					duplicated.mOnlyUseNumberOfChannelForDefineType = true;			//the calcul of the type will be on the number of declared channel, and not on the last channel founded.
					

					//and, we add for data fusion in the first declaration, in fonction of mask.
					mHlslCode += tabInc + registerName +"."+ sv2.at(2) +" = "+ registerName + "_" + std::to_string(input.mNbInputDeclaration) +";\n";

					input.mNbInputDeclaration++;
					mListShaderRegistrer_VertexInput.push_back(duplicated);
				}

				

			}else if (mShaderDeclarationPart == SDP_OutputSignature) {


				sv2 = split(line.substr(3), ' ');
				for (size_t j = 0; j < sv2.size(); j++)
				{
					if (sv2.at(j) == "")
					{
						sv2.erase(sv2.begin() + j);
						j--;
					}
				}

				bool isTextcoord = (sv2.at(0) == "TEXCOORD");
				bool isTarget = (sv2.at(0) == "SV_TARGET");
				bool isColor = (sv2.at(0) == "COLOR");

				string registerName = ((sv2.at(3).substr(0, 1) != "o") ? "o" : "") + sv2.at(3);


				size_t isfound = (size_t)-1;
				size_t nbinput = mListShaderRegistrer_Ouput.size();
				for (size_t j = 0; j < nbinput; j++)
				{
					if (mListShaderRegistrer_Ouput.at(j).mName == registerName)
					{
						isfound = j;
						break;
					}
				}

				if (isfound == (size_t)-1)
				{
					mListShaderRegistrer_Ouput.push_back(ShaderRegistrer(registerName));
					mListShaderRegistrer_Ouput.back().addChannels(sv2.at(2));
					textCoordIndexInc_Output = sv2.at(1);
					mListShaderRegistrer_Ouput.back().mInputName = sv2.at(0) + (((isTextcoord || isColor) && (sv2.at(1) != "0")) ? sv2.at(1) : "") + (((isTarget) && (targetIndexInc_Output != 0)) ? std::to_string(targetIndexInc_Output) : "");
				}else{

					//we get the first declaration
					ShaderRegistrer &ouput = mListShaderRegistrer_Ouput.at(isfound);
					ouput.mOnlyUseNumberOfChannelForDefineType = true;
					ShaderRegistrer duplicated(registerName + "_" + std::to_string(ouput.mNbInputDeclaration));
					duplicated.addChannels(sv2.at(2));
					duplicated.mOnlyUseNumberOfChannelForDefineType = true;
					textCoordIndexInc_Output = sv2.at(1);
					duplicated.mInputName = sv2.at(0) + (((isTextcoord || isColor) && (sv2.at(1) != "0")) ? sv2.at(1) : "");

					ouput.mNbInputDeclaration++;
					mListShaderRegistrer_Ouput.push_back(duplicated);
				}

				if (isTarget)
					targetIndexInc_Output++;
			}

			continue;
		}

		if (line.substr(0, 6) == "vs_5_0")
			continue;
		if (line.substr(0, 6) == "ps_5_0")
		{
			isAPixelShader = true;
			continue;
		}

		if (line.substr(0, 4) == "dcl_")
		{
			continue;
		}

		sv2 = split(line, ' ');

		std::string instruction = "";
		std::string argumentsLine = "";

		bool isfirst = true;
		size_t nbElement = sv2.size();
		for (size_t j = 0; j < nbElement; j++)
		{
			if ((sv2.at(j) == "") || (sv2.at(j) == "\t"))
				continue;

			if (isfirst)
			{
				instruction = sv2.at(j);
				isfirst = false;

				if (instruction == "ld_structured_indexable(structured_buffer,")
				{
					instruction += " " + sv2.at(j + 1);
					sv2.erase(sv2.begin() + j);
					nbElement--;
					j--;
				}
			}
			else {
				argumentsLine += sv2.at(j);
			}
		}

		if (isfirst)
			continue;

		
		




		//-------- search for custom case : Modifier's Instructions (cf https://msdn.microsoft.com/en-us/library/windows/desktop/hh447141(v=vs.85).aspx)
		bool useAbs = false;
		bool useSaturate = false;
		if (instruction.length() > 4)
		{
			if (instruction.substr(instruction.length() - 4) == "_abs")
			{
				useAbs = true;
				instruction = instruction.substr(0, instruction.length() - 4);
			}
			if (instruction.substr(instruction.length() - 4) == "_sat")
			{
				useSaturate = true;
				instruction = instruction.substr(0, instruction.length() - 4);
			}
		}



		//-------- Split of line into arguments
		sv2 = split(argumentsLine, ',');					//argument's list
		
		bool isStartedParenthese = false;
		
		for (size_t j = 0; j < sv2.size(); j++)						//case of  l(x, x, x.x, x) witch was exploded by the split. we have to group. Hyp: there is no inclusion : l(x, l(x.x, x) )
		{
			if(!isStartedParenthese)
			{
				if ((sv2.at(j).substr(0, 2) == "l(") && (!(sv2.at(j).substr(sv2.at(j).length() - 1) == ")")) )
					isStartedParenthese = true;

			}else{

				sv2.at(j - 1) += "," + sv2.at(j);

				sv2.erase(sv2.begin() + j);
				j--;

				if ( sv2.at(j).substr(sv2.at(j).length()-1) == ")" )
					isStartedParenthese = false;
			}
		}
		size_t nbArg = sv2.size();

		//now, we will replace l(, par la declaration floatX(
		for (size_t j = 0; j < nbArg; j++)
		{
			if (sv2.at(j).substr(0, 2) == "l(")
			{
				sv3 = split(sv2.at(j), ',');

				if (sv3.size() != 1)
				{
					if ((instruction == "and") && (nbArg == 3))			//in certain cases, better to use intX for compilation.
						sv2.at(j) = "int" + std::to_string(sv3.size()) + "(" + sv2.at(j).substr(2);
					else
						sv2.at(j) = "float" + std::to_string(sv3.size()) + "(" + sv2.at(j).substr(2);
				}else {
					sv2.at(j) = sv2.at(j).substr(2, sv2.at(j).length()-3);
				}

				//hack problem of precision on float
				if( ( ((baseFilename.length()>19)&&(baseFilename.substr(0,19)=="T1_C_BUMP_DYN_S_W_U")) || ((baseFilename.length()>9) && (baseFilename.substr(0, 9) == "T1_C_DYN_")))&&(sv2.at(j)=="float4(0.000000,100.000000,100.000000,0.000000)"))		//big hack to avoid the only case where there is a division by zero, the compilator don't like it at all.
					sv2.at(j) = "float4(0.0000001,100.000000,100.000000,0.000000)";
				if ((baseFilename.length() >= 20) && (baseFilename.substr(0, 20) == "VelocityStretch_W_VS"))
				{
					if (sv2.at(j) == "float4(0.000000,0.000000,6.250000,6.250000)")
						sv2.at(j) = "float4(0.0000001,0.0000001,6.250000,6.250000)";
					if (sv2.at(j) == "float4(0.000000,0.000000,1.000000,1.000000)")
						sv2.at(j) = "float4(0.0000001,0.0000001,1.000000,1.000000)";
				}

				
			}
		}
		

		
		//we will replace "cbX[Y]" by theirs declaration of cbuffer "g_mWVP_VS[Y]"
		size_t nbCbuf = mListShaderCBuffer.size();
		for (size_t j = 0; j < nbArg; j++)
		{
			if ((sv2.at(j).substr(0, 2) != "cb") && (sv2.at(j).substr(0, 3) != "-cb") )
				continue;

			bool isNegative = (sv2.at(j).substr(0, 1) == "-");
			if (isNegative)
				sv2.at(j) = sv2.at(j).substr(1);

			size_t index = sv2.at(j).substr(2).find('[');
			if (index != std::string::npos)
			{
				std::string cbName = sv2.at(j).substr(0, 2 + index);

				for (size_t k = 0; k < nbCbuf; k++)
				{
					if (mListShaderCBuffer.at(k).mCodeName == cbName)
					{
						std::string insisde = sv2.at(j).substr(2 + index + 1);
						std::string onright = "";
						
						size_t index2 = insisde.find_last_of(']');
						if (index2 != std::string::npos)
						{
							onright = insisde.substr(index2 + 1);
							insisde = insisde.substr(0, index2);
						}

						
						sv2.at(j) = mListShaderCBuffer.at(k).getArgumentString(insisde, onright);
						break;
					}
				}

			}

			if (isNegative)
				sv2.at(j) = "-"+ sv2.at(j);
		}

		// replace |r6.xyzx| by abs( r6.xyzx), may be it's better to have length( r6.xyzx), have to try.
		nbCbuf = mListShaderCBuffer.size();
		for (size_t j = 0; j < nbArg; j++)
		{
			sv3 = split(sv2.at(j), '|');

			if (sv3.size() != 3)
				continue;

			sv2.at(j) = sv3.at(0) + "abs(" + sv3.at(1) + ")" + sv3.at(2);
		}



		// debug current instruction.
		mHlslCode += tabInc + "//" + line +"\n";



		//--------------------------- Gestion of Instructions
		registreResult.clear();
		
		
		//------ Block
		if ((instruction == "if_nz") && (nbArg == 1))										// if any bit in r0.x is nonzero ( https://msdn.microsoft.com/fr-fr/library/windows/desktop/hh447105(v=vs.85).aspx )
		{
			mHlslCode += tabInc + "if(" + sv2.at(0) + ")\n";
			mHlslCode += tabInc + "{\n";
			tabInc += "\t";

		}else if(instruction == "else") {

			tabInc = tabInc.substr(0, tabInc.length()-1);
			mHlslCode += tabInc + "}else{\n";
			tabInc += "\t";

		}else if (instruction == "endif") {

			tabInc = tabInc.substr(0, tabInc.length() - 1);
			mHlslCode += tabInc + "}\n";



		}else if (instruction == "loop"){
		
			mHlslCode += tabInc + "while(true)\n";
			mHlslCode += tabInc + "{\n";
			tabInc += "\t";
		}else if ((instruction == "breakc_z") && (nbArg == 1)) {								// https://msdn.microsoft.com/en-us/library/windows/desktop/hh446841(v=vs.85).aspx

			mHlslCode += tabInc + "if (" + sv2.at(0) + " == 0) break;\n";
		
		}else if ((instruction == "breakc_nz") && (nbArg == 1)) {								// https://msdn.microsoft.com/en-us/library/windows/desktop/hh446841(v=vs.85).aspx

			mHlslCode += tabInc + "if (" + sv2.at(0) + " != 0) break;\n";

		}else if (instruction == "endloop") {

			tabInc = tabInc.substr(0, tabInc.length() - 1);
			mHlslCode += tabInc + "}\n";

		}else if (instruction == "ret") {											//end of the shader;









		//------ Component Wise but could be apply directly
		}else if ((instruction == "mov") && (nbArg == 2)) {

			//rectifyChannels try to solve the bug on channel "mov r0.zw, v2.xxxy" must be "r0.zw = (v2.xxxy).zw;" in Hlsl. "r0.zw = v2.xxxy;" doesn't work.
			string ouputChannel = rectifyChannels(sv2.at(0));
			bool haveToForce = (ouputChannel.length() > 1);

			mHlslCode += tabInc + sv2.at(0) + " = " + (haveToForce ? "(" : "") + sv2.at(1) + (haveToForce ? (")."+ ouputChannel) : "") + ";\n";
			registreResult.push_back(sv2.at(0));


		}else if (((instruction == "add") || (instruction == "iadd")) && (nbArg == 3)) {

			string ouputChannel = rectifyChannels(sv2.at(0));
			bool haveToForce = (ouputChannel.length() > 1);

			mHlslCode += tabInc + sv2.at(0) + " = " + (haveToForce ? "(" : "") + ((instruction == "iadd") ? "floor(" : "") + sv2.at(1) + " + " + sv2.at(2) + ((instruction == "iadd") ? ")" : "") + (haveToForce ? (")." + ouputChannel) : "") + ";\n";
			registreResult.push_back(sv2.at(0));

		}else if ((instruction == "mul") && (nbArg == 3)) {								// https://msdn.microsoft.com/en-us/library/windows/desktop/hh447193(v=vs.85).aspx

			string ouputChannel = rectifyChannels(sv2.at(0));
			bool haveToForce = (ouputChannel.length() > 1);

			mHlslCode += tabInc + sv2.at(0) + " = " + (haveToForce ? "(" : "") + sv2.at(1) + " * " + sv2.at(2) + (haveToForce ? (")." + ouputChannel) : "") + ";\n";
			registreResult.push_back(sv2.at(0));

		}else if (((instruction == "mad") || (instruction == "imad")) && (nbArg == 4)) {									//https://msdn.microsoft.com/en-us/library/windows/desktop/hh447183(v=vs.85).aspx

			string ouputChannel = rectifyChannels(sv2.at(0));
			bool haveToForce = (ouputChannel.length() > 1);

			mHlslCode += tabInc + sv2.at(0) + " = " + (haveToForce ? "(" : "") + ((instruction == "imad") ? "floor(" : "") + sv2.at(1) + " * " + sv2.at(2) + " + " + sv2.at(3) + ((instruction == "imad") ? ")" : "") + (haveToForce ? (")." + ouputChannel) : "") + ";\n";
			registreResult.push_back(sv2.at(0));

		}else if ((instruction == "div") && (nbArg == 3)) {								// https://msdn.microsoft.com/en-us/library/windows/desktop/hh446971(v=vs.85).aspx

			string ouputChannel = rectifyChannels(sv2.at(0));
			bool haveToForce = (ouputChannel.length() > 1);

			mHlslCode += tabInc + sv2.at(0) + " = " + (haveToForce ? "(" : "") + sv2.at(1) + " / " + sv2.at(2) + (haveToForce ? (")." + ouputChannel) : "") + ";\n";
			registreResult.push_back(sv2.at(0));

		}else if ((instruction == "rsq") && (nbArg == 2)) {

			string ouputChannel = rectifyChannels(sv2.at(0));
			bool haveToForce = (ouputChannel.length() > 1);

			mHlslCode += tabInc + sv2.at(0) + " = " + (haveToForce ? "(" : "") +" 1.0/sqrt(" + sv2.at(1) + ") " + (haveToForce ? (")." + ouputChannel) : "") + ";\n";
			registreResult.push_back(sv2.at(0));

		}else if ((instruction == "sqrt") && (nbArg == 2)) {

			string ouputChannel = rectifyChannels(sv2.at(0));
			bool haveToForce = (ouputChannel.length() > 1);
			
			mHlslCode += tabInc + sv2.at(0) + " = " + (haveToForce ? "(" : "") + " sqrt(" + sv2.at(1) + ") " + (haveToForce ? (")." + ouputChannel) : "") + ";\n";
			registreResult.push_back(sv2.at(0));

		}else if ((instruction == "max") && (nbArg == 3)) {

			string ouputChannel = rectifyChannels(sv2.at(0));
			bool haveToForce = (ouputChannel.length() > 1);

			mHlslCode += tabInc + sv2.at(0) + " = " + (haveToForce ? "(" : "") + " max(" + sv2.at(1) + "," + sv2.at(2) + ") " + (haveToForce ? (")." + ouputChannel) : "") + ";\n";
			registreResult.push_back(sv2.at(0));

		}else if ((instruction == "min") && (nbArg == 3)) {

			string ouputChannel = rectifyChannels(sv2.at(0));
			bool haveToForce = (ouputChannel.length() > 1);
			
			mHlslCode += tabInc + sv2.at(0) + " = " + (haveToForce ? "(" : "") + " min(" + sv2.at(1) + "," + sv2.at(2) + ") " + (haveToForce ? (")." + ouputChannel) : "") + ";\n";
			registreResult.push_back(sv2.at(0));


		}else if ((instruction == "exp") && (nbArg == 2)) {

			string ouputChannel = rectifyChannels(sv2.at(0));
			bool haveToForce = (ouputChannel.length() > 1);

			mHlslCode += tabInc + sv2.at(0) + " = " + (haveToForce ? "(" : "") + " pow(2, " + sv2.at(1) + ") " + (haveToForce ? (")." + ouputChannel) : "") + ";\n";
			registreResult.push_back(sv2.at(0));

		}
		else if ((instruction == "log") && (nbArg == 2)) {

			string ouputChannel = rectifyChannels(sv2.at(0));
			bool haveToForce = (ouputChannel.length() > 1);
			
			mHlslCode += tabInc + sv2.at(0) + " = " + (haveToForce ? "(" : "") + " log2(" + sv2.at(1) + ") " + (haveToForce ? (")." + ouputChannel) : "") + ";\n";
			registreResult.push_back(sv2.at(0));

		}else if (((instruction == "deriv_rtx_coarse") || (instruction == "deriv_rtx_fine")) && (nbArg == 2)) {										//https://msdn.microsoft.com/en-us/library/windows/desktop/hh446948(v=vs.85).aspx

			string ouputChannel = rectifyChannels(sv2.at(0));
			bool haveToForce = (ouputChannel.length() > 1);
			
			mHlslCode += tabInc + sv2.at(0) + " = " + (haveToForce ? "(" : "") + " ddx(" + sv2.at(1) + ") " + (haveToForce ? (")." + ouputChannel) : "") + ";\n";
			registreResult.push_back(sv2.at(0));

		}else if (((instruction == "deriv_rty_coarse") || (instruction == "deriv_rty_fine")) && (nbArg == 2)) {										//https://msdn.microsoft.com/en-us/library/windows/desktop/hh446948(v=vs.85).aspx

			string ouputChannel = rectifyChannels(sv2.at(0));
			bool haveToForce = (ouputChannel.length() > 1);
			
			mHlslCode += tabInc + sv2.at(0) + " = " + (haveToForce ? "(" : "") +  "ddy(" + sv2.at(1) + ") " + (haveToForce ? (")." + ouputChannel) : "") + ";\n";
			registreResult.push_back(sv2.at(0));

		}else if ((instruction == "frc") && (nbArg == 2)) {

			
			string ouputChannel = rectifyChannels(sv2.at(0));
			bool haveToForce = (ouputChannel.length() > 1);
			
			mHlslCode += tabInc + "tmp = floor(" + sv2.at(1) + ");\n";
			mHlslCode += tabInc + "if(ceil(" + sv2.at(1) + ") - " + sv2.at(1) + "< " + sv2.at(1) + " - tmp)\n";
			mHlslCode += tabInc + "\ttmp = ceil(" + sv2.at(1) + "); \n";
			mHlslCode += tabInc + sv2.at(0) + " = " + (haveToForce ? "(" : "") + sv2.at(1) + " - tmp" + (haveToForce ? (")." + ouputChannel) : "") + ";\n";
			registreResult.push_back(sv2.at(0));












		//------ Interger Special
		}else if (((instruction == "itof") || (instruction == "ftoi") || (instruction == "utof")) && (nbArg == 2)) {			//in Shader Model 3.0 there is not int.

			mHlslCode += tabInc + sv2.at(0) + " = " + ((instruction == "ftoi") ? "floor(" : "") + sv2.at(1) + ((instruction == "ftoi") ? ")" : "") + ";\n";
			registreResult.push_back(sv2.at(0));

		}else if ((instruction == "imul") && (nbArg == 4)) {								//Signed integer multiply ( https://msdn.microsoft.com/en-us/library/windows/desktop/hh447138(v=vs.85).aspx )

			string destHI = sv2.at(0);
			string destLO = sv2.at(1);
			string src0 = sv2.at(2);
			string src1 = sv2.at(3);

			//first difficulties : the mulplication of two sources 32bits with a result witch could be on 64 bits. So, we use 2 registrer and a special command to do the operation.
			//but, it's too complicate for now.

			if (destHI != "null")
			{
				mHlslCode += tabInc + destHI + " = (" + src0 + " * " + src1 + ")/4294967295;\n";
				registreResult.push_back(destHI);
			}else if (destLO != "null") {
				mHlslCode += tabInc + destLO + " = " + src0 + " * " + src1 + ";\n";
				registreResult.push_back(destLO);
			}









		
		
		
		


		//------ Component and Bit wise. TODO doing better by using shader's function, with some muplication/division by 2.
		}else if ((instruction == "and") && (nbArg == 3)) {										//https://msdn.microsoft.com/en-us/library/windows/desktop/hh446818(v=vs.85).aspx

			//mHlslCode += tabInc + sv2.at(0) + " = " + sv2.at(1) + " & " + sv2.at(2) + ";\t\t//TODO bit-wise operation\n";	
			
			//v0
			//mHlslCode += tabInc + sv2.at(0) + " = "+ ((sv2.at(1).substr(0,3)!="int") ? "(int)": "") + sv2.at(1) + " && "+ ((sv2.at(2).substr(0, 3) != "int") ? "(int)" : "") + sv2.at(2) + ";\t\t//TODO bit-wise operation\n";
			//registreResult.push_back(sv2.at(0));

			Shader_AsmArg arg0(sv2.at(0), getShaderCBuffer_Var(sv2.at(0)));
			Shader_AsmArg arg1(sv2.at(1), getShaderCBuffer_Var(sv2.at(1)));
			Shader_AsmArg arg2(sv2.at(2), getShaderCBuffer_Var(sv2.at(2)));

			if (arg0.haveNoChannel())
			{
				//mHlslCode += tabInc + sv2.at(0) + " = " + ((sv2.at(1).substr(0, 3) != "int") ? "(int)" : "") + sv2.at(1) + " && " + ((sv2.at(2).substr(0, 3) != "int") ? "(int)" : "") + sv2.at(2) + ";\t\t//TODO bit-wise operation\n";
				//mHlslCode += tabInc + sv2.at(0) + " = specialCheck_Dbxv2Shaders(and_BitWise_operation(" + ((sv2.at(1).substr(0, 3) != "int") ? "(int)" : "") + sv2.at(1) + " , " + ((sv2.at(2).substr(0, 3) != "int") ? "(int)" : "") + sv2.at(2) + "));\t\t// bit-wise operation\n";
				mHlslCode += tabInc + sv2.at(0) + " = specialCheck_Dbxv2Shaders(and_BitWise_operation(" + sv2.at(1) + " , " + sv2.at(2) + "));\t\t// bit-wise operation\n";
			}else{

				string ouputChannel = rectifyChannels(sv2.at(0));
				bool haveToForce = (ouputChannel.length() > 1);

				size_t nbChannel = (!haveToForce) ? arg0.mListChannel.size() : ouputChannel.length();
				for (size_t j = 0; j < nbChannel; j++)
				{
					if(!haveToForce)
						mHlslCode += tabInc + arg0.getArg_ForChannel(j) + " = specialCheck_Dbxv2Shaders(and_BitWise_operation(" + arg1.getArg_ForChannel(j) + " , " +  arg2.getArg_ForChannel(j) + "));\t\t// bit-wise operation\n";
					else
						mHlslCode += tabInc + arg0.getArg_ForChannel(j) + " = specialCheck_Dbxv2Shaders(and_BitWise_operation( (" + sv2.at(1) + ")."+ ouputChannel.substr(j,1) +" , (" + sv2.at(2) + ")."+ ouputChannel.substr(j, 1) +"));\t\t// bit-wise operation\n";
				}
			}
			registreResult.push_back(sv2.at(0));



		}else if ((instruction == "or") && (nbArg == 3)) {										//https://msdn.microsoft.com/en-us/library/windows/desktop/hh447202(v=vs.85).aspx
																								//mHlslCode += tabInc + sv2.at(0) + " = " + sv2.at(1) + " | " + sv2.at(2) + ";\t\t//TODO bit-wise operation\n";
			//mHlslCode += tabInc + sv2.at(0) + " = " + ((sv2.at(1).substr(0, 3) != "int") ? "(int)" : "") + sv2.at(1) + " || " + ((sv2.at(2).substr(0, 3) != "int") ? "(int)" : "") + sv2.at(2) + ";\t\t//TODO bit-wise operation\n";
			//registreResult.push_back(sv2.at(0));

			Shader_AsmArg arg0(sv2.at(0), getShaderCBuffer_Var(sv2.at(0)));
			Shader_AsmArg arg1(sv2.at(1), getShaderCBuffer_Var(sv2.at(1)));
			Shader_AsmArg arg2(sv2.at(2), getShaderCBuffer_Var(sv2.at(2)));

			if (arg0.haveNoChannel())
			{
				mHlslCode += tabInc + sv2.at(0) + " = specialCheck_Dbxv2Shaders(or_BitWise_operation(" + sv2.at(1) + " , " + sv2.at(2) + "));\t\t// bit-wise operation\n";
			}else {
				string ouputChannel = rectifyChannels(sv2.at(0));
				bool haveToForce = (ouputChannel.length() > 1);

				size_t nbChannel = (!haveToForce) ? arg0.mListChannel.size() : ouputChannel.length();
				for (size_t j = 0; j < nbChannel; j++)
				{
					if (!haveToForce)
						mHlslCode += tabInc + arg0.getArg_ForChannel(j) + " = specialCheck_Dbxv2Shaders(or_BitWise_operation(" + arg1.getArg_ForChannel(j) + " , " + arg2.getArg_ForChannel(j) + "));\t\t// bit-wise operation\n";
					else
						mHlslCode += tabInc + arg0.getArg_ForChannel(j) + " = specialCheck_Dbxv2Shaders(or_BitWise_operation( (" + sv2.at(1) + ")." + ouputChannel.substr(j, 1) + " , (" + sv2.at(2) + ")." + ouputChannel.substr(j, 1) + "));\t\t// bit-wise operation\n";
				}
			}
			registreResult.push_back(sv2.at(0));


		}else if ((instruction == "ineg") && (nbArg == 2)) {									//https://msdn.microsoft.com/en-us/library/windows/desktop/hh447139(v=vs.85).aspx

			mHlslCode += tabInc + sv2.at(0) + " = -" + sv2.at(1) +";\t\t// bit-wise operation\n";	// component at 2  => negative.
			registreResult.push_back(sv2.at(0));

		}else if ((instruction == "imax") && (nbArg == 3)) {										//https://msdn.microsoft.com/en-us/library/windows/desktop/hh446825(v=vs.85).aspx
																								//mHlslCode += tabInc + sv2.at(0) + " = " + sv2.at(1) + " | " + sv2.at(2) + ";\t\t//TODO bit-wise operation\n";
			//mHlslCode += tabInc + sv2.at(0) + " = max("+ sv2.at(0) +","+ sv2.at(2) +");\t\t//TODO bit-wise operation\n";
			//registreResult.push_back(sv2.at(0));

			Shader_AsmArg arg0(sv2.at(0), getShaderCBuffer_Var(sv2.at(0)));
			Shader_AsmArg arg1(sv2.at(1), getShaderCBuffer_Var(sv2.at(1)));
			Shader_AsmArg arg2(sv2.at(2), getShaderCBuffer_Var(sv2.at(2)));

			if (arg0.haveNoChannel())
			{
				mHlslCode += tabInc + sv2.at(0) + " = max(" + sv2.at(1) + " , " + sv2.at(2) + "));\t\t// bit-wise operation\n";
			}else {
				string ouputChannel = rectifyChannels(sv2.at(0));
				bool haveToForce = (ouputChannel.length() > 1);

				size_t nbChannel = (!haveToForce) ? arg0.mListChannel.size() : ouputChannel.length();
				for (size_t j = 0; j < nbChannel; j++)
				{
					if (!haveToForce)
						mHlslCode += tabInc + arg0.getArg_ForChannel(j) + " = max(" + arg1.getArg_ForChannel(j) + " , " + arg2.getArg_ForChannel(j) + ");\t\t// bit-wise operation\n";
					else
						mHlslCode += tabInc + arg0.getArg_ForChannel(j) + " = max( (" + sv2.at(1) + ")." + ouputChannel.substr(j, 1) + " , (" + sv2.at(2) + ")." + ouputChannel.substr(j, 1) + ");\t\t// bit-wise operation\n";
				}
			}
			registreResult.push_back(sv2.at(0));


		





		//------ Component Wise, have to be apply separatly.

		}else if (((instruction == "dp2") || (instruction == "dp3") || (instruction == "dp4")) && (nbArg == 3)) {

			std::string::size_type sz;
			size_t nbChannelDot = std::stoi(instruction.substr(2, 1).c_str(), &sz);


			Shader_AsmArg arg1(sv2.at(1), getShaderCBuffer_Var(sv2.at(1)));
			Shader_AsmArg arg2(sv2.at(2), getShaderCBuffer_Var( split(sv2.at(2),'.').at(0) ));

			mHlslCode += tabInc + sv2.at(0) + " = dot(" + arg1.getArg(nbChannelDot) + "," + arg2.getArg(nbChannelDot) + ");\n";
			registreResult.push_back(sv2.at(0));


		}else if ((instruction == "ftou") && (nbArg == 2)) {									// https://msdn.microsoft.com/en-us/library/windows/desktop/hh447078(v=vs.85).aspx

			Shader_AsmArg arg0(sv2.at(0), getShaderCBuffer_Var(sv2.at(0)));
			Shader_AsmArg arg1(sv2.at(1), getShaderCBuffer_Var(sv2.at(1)));

			if(arg0.haveNoChannel())
			{
				mHlslCode += tabInc + "tmp = floor(" + arg1.getArg() + ");\n";
				mHlslCode += tabInc + "if(ceil(" + arg1.getArg() + ") - " + arg1.getArg() + "< " + arg1.getArg() + " - tmp)\n";
				mHlslCode += tabInc + "\ttmp = ceil(" + arg1.getArg() + "); \n";
				mHlslCode += tabInc + arg0.getArg() + " = tmp;\n";

			}else {

				size_t nbChannel = arg0.mListChannel.size();
				for (size_t j = 0; j < nbChannel; j++)
				{
					mHlslCode += tabInc + "tmp = floor(" + arg1.getArg_ForChannel(j) + ");\n";
					mHlslCode += tabInc + "if(ceil(" + arg1.getArg_ForChannel(j) + ") - " + arg1.getArg_ForChannel(j) + "< " + arg1.getArg_ForChannel(j) + " - tmp)\n";
					mHlslCode += tabInc + "\ttmp = ceil(" + arg1.getArg_ForChannel(j) + "); \n";
					mHlslCode += tabInc + arg0.getArg_ForChannel(j) + " = tmp;\n";
				}
			}
			registreResult.push_back(sv2.at(0));

			
		}else if (((instruction == "eq")|| (instruction == "ieq")) && (nbArg == 3)) {									// https://msdn.microsoft.com/en-us/library/windows/desktop/hh447181(v=vs.85).aspx


			Shader_AsmArg arg0(sv2.at(0), getShaderCBuffer_Var(sv2.at(0)));
			Shader_AsmArg arg1(sv2.at(1), getShaderCBuffer_Var(sv2.at(1)));
			Shader_AsmArg arg2(sv2.at(2), getShaderCBuffer_Var(sv2.at(2)));

			if (arg0.haveNoChannel())
			{
				mHlslCode += tabInc + arg0.getArg() + " = ((" + arg1.getArg() + " == " + arg2.getArg() + ") ? 0xFFFFFFFF : 0.0);\n";
			}else{
				size_t nbChannel = arg0.mListChannel.size();
				for (size_t j = 0; j < nbChannel; j++)
					mHlslCode += tabInc + arg0.getArg_ForChannel(j) + " = ((" + arg1.getArg_ForChannel(j) + " == " + arg2.getArg_ForChannel(j) + ") ? 0xFFFFFFFF : 0.0); \n";
			}
			registreResult.push_back(sv2.at(0));


		}else if (((instruction == "ne") || (instruction == "ine")) && (nbArg == 3)) {									// https://msdn.microsoft.com/en-us/library/windows/desktop/hh447181(v=vs.85).aspx


			Shader_AsmArg arg0(sv2.at(0), getShaderCBuffer_Var(sv2.at(0)));
			Shader_AsmArg arg1(sv2.at(1), getShaderCBuffer_Var(sv2.at(1)));
			Shader_AsmArg arg2(sv2.at(2), getShaderCBuffer_Var(sv2.at(2)));

			if (arg0.haveNoChannel())
			{
				mHlslCode += tabInc + arg0.getArg() + " = ((" + arg1.getArg() + " != " + arg2.getArg() + ") ? 0xFFFFFFFF : 0.0);\n";
			}else {
				size_t nbChannel = arg0.mListChannel.size();
				for (size_t j = 0; j < nbChannel; j++)
					mHlslCode += tabInc + arg0.getArg_ForChannel(j) + " = ((" + arg1.getArg_ForChannel(j) + " != " + arg2.getArg_ForChannel(j) + ") ? 0xFFFFFFFF : 0.0); \n";
			}
			registreResult.push_back(sv2.at(0));


		}else if (((instruction == "ge") || (instruction == "ige")) && (nbArg == 3)) {									// https://msdn.microsoft.com/en-us/library/windows/desktop/hh447181(v=vs.85).aspx



			Shader_AsmArg arg0(sv2.at(0), getShaderCBuffer_Var(sv2.at(0)));
			Shader_AsmArg arg1(sv2.at(1), getShaderCBuffer_Var(sv2.at(1)));
			Shader_AsmArg arg2(sv2.at(2), getShaderCBuffer_Var(sv2.at(2)));

			if (arg0.haveNoChannel())
			{
				mHlslCode += tabInc + arg0.getArg() + " = ((" + arg1.getArg() + " >= " + arg2.getArg() + ") ? 0xFFFFFFFF : 0.0);\n";
			}else {
				size_t nbChannel = arg0.mListChannel.size();
				for (size_t j = 0; j < nbChannel; j++)
					mHlslCode += tabInc + arg0.getArg_ForChannel(j) + " = ((" + arg1.getArg_ForChannel(j) + " >= " + arg2.getArg_ForChannel(j) + ") ? 0xFFFFFFFF : 0.0); \n";
			}
			registreResult.push_back(sv2.at(0));


		}else if ((instruction == "lt") && (nbArg == 3)) {									// https://msdn.microsoft.com/en-us/library/windows/desktop/hh447181(v=vs.85).aspx


			Shader_AsmArg arg0(sv2.at(0), getShaderCBuffer_Var(sv2.at(0)));
			Shader_AsmArg arg1(sv2.at(1), getShaderCBuffer_Var(sv2.at(1)));
			Shader_AsmArg arg2(sv2.at(2), getShaderCBuffer_Var(sv2.at(2)));

			if (arg0.haveNoChannel())
			{
				mHlslCode += tabInc + arg0.getArg() + " = ((" + arg1.getArg() + " < " + arg2.getArg() + ") ? 0xFFFFFFFF : 0.0);\n";
			}else {
				size_t nbChannel = arg0.mListChannel.size();
				for (size_t j = 0; j < nbChannel; j++)
					mHlslCode += tabInc + arg0.getArg_ForChannel(j) + " = ((" + arg1.getArg_ForChannel(j) + " < " + arg2.getArg_ForChannel(j) + ") ? 0xFFFFFFFF : 0.0); \n";
			}
			registreResult.push_back(sv2.at(0));




		}else if (((instruction == "round_ne") || (instruction == "round_ni") || (instruction == "round_pi") || (instruction == "round_z")) && (nbArg == 2)) {									// https://msdn.microsoft.com/en-us/library/windows/desktop/hh447217(v=vs.85).aspx


			Shader_AsmArg arg0(sv2.at(0), getShaderCBuffer_Var(sv2.at(0)));
			Shader_AsmArg arg1(sv2.at(1), getShaderCBuffer_Var(sv2.at(1)));

			if (arg0.haveNoChannel())
			{
				mHlslCode += tabInc + arg0.getArg() + " = round(" + arg1.getArg() + ");\n";
			}
			else {
				size_t nbChannel = arg0.mListChannel.size();
				for (size_t j = 0; j < nbChannel; j++)
					mHlslCode += tabInc + arg0.getArg_ForChannel(j) + " = round(" + arg1.getArg_ForChannel(j) + ");\n";
			}
			registreResult.push_back(sv2.at(0));


		}else if ((instruction == "udiv") && (nbArg == 4)) {									// https://msdn.microsoft.com/en-us/library/windows/desktop/hh447244(v=vs.85).aspx


			Shader_AsmArg arg0(sv2.at(0), getShaderCBuffer_Var(sv2.at(0)));
			Shader_AsmArg arg1(sv2.at(1), getShaderCBuffer_Var(sv2.at(1)));
			Shader_AsmArg arg2(sv2.at(2), getShaderCBuffer_Var(sv2.at(2)));
			Shader_AsmArg arg3(sv2.at(3), getShaderCBuffer_Var(sv2.at(3)));

			if (arg0.haveNoChannel())
			{
				mHlslCode += tabInc + arg0.getArg() + " = floor(" + arg2.getArg() + " / " + arg3.getArg() + ");\n";
				if (sv2.at(1) != "null")
					mHlslCode += tabInc + arg1.getArg() + " = " + arg2.getArg() + " - (" + arg0.getArg() + " * " + arg3.getArg() + ");\n";
			}else {
				size_t nbChannel = arg0.mListChannel.size();
				for (size_t j = 0; j < nbChannel; j++)
				{
					mHlslCode += tabInc + arg0.getArg_ForChannel(j) + " = floor(" + arg2.getArg_ForChannel(j) + " / " + arg3.getArg_ForChannel(j) + ");\n";
					if (sv2.at(1) != "null")
						mHlslCode += tabInc + arg1.getArg_ForChannel(j) + " = " + arg2.getArg_ForChannel(j) + " - (" + arg0.getArg_ForChannel(j) + " * " + arg3.getArg_ForChannel(j) + ");\n";
				}
			}
			registreResult.push_back(sv2.at(0));
			if (sv2.at(1) != "null")
				registreResult.push_back(sv2.at(1));


		}else if ((instruction == "movc") && (nbArg == 4)) {									// https://msdn.microsoft.com/en-us/library/windows/desktop/hh447189(v=vs.85).aspx

			
			Shader_AsmArg arg0(sv2.at(0), getShaderCBuffer_Var(sv2.at(0)));
			Shader_AsmArg arg1(sv2.at(1), getShaderCBuffer_Var(sv2.at(1)));
			Shader_AsmArg arg2(sv2.at(2), getShaderCBuffer_Var(sv2.at(2)));
			Shader_AsmArg arg3(sv2.at(3), getShaderCBuffer_Var(sv2.at(3)));

			if (arg0.haveNoChannel())
			{
				mHlslCode += tabInc + "if(" + arg1.getArg() + ")\n";
				mHlslCode += tabInc + "\t" + arg0.getArg() + " = " + arg2.getArg() + ";\n";
				mHlslCode += tabInc + "else\n";
				mHlslCode += tabInc + "\t" + arg0.getArg() + " = " + arg3.getArg() + ";\n";
			}else {
				size_t nbChannel = arg0.mListChannel.size();
				for (size_t j = 0; j < nbChannel; j++)
				{
					mHlslCode += tabInc + "if(" + arg1.getArg_ForChannel(j) + ")\n";
					mHlslCode += tabInc + "\t" + arg0.getArg_ForChannel(j) + " = " + arg2.getArg_ForChannel(j) + ";\n";
					mHlslCode += tabInc + "else\n";
					mHlslCode += tabInc + "\t" + arg0.getArg_ForChannel(j) + " = " + arg3.getArg_ForChannel(j) + ";\n";
				}
			}
			registreResult.push_back(sv2.at(0));
		

		}else if ((instruction == "ushr") && (nbArg == 3)) {									// https://msdn.microsoft.com/en-us/library/windows/desktop/hh447251(v=vs.85).aspx

			
			Shader_AsmArg arg0(sv2.at(0), getShaderCBuffer_Var(sv2.at(0)));
			Shader_AsmArg arg1(sv2.at(1), getShaderCBuffer_Var(sv2.at(1)));
			Shader_AsmArg arg2(sv2.at(2), getShaderCBuffer_Var(sv2.at(2)));

			if (arg0.haveNoChannel())
			{
				mHlslCode += tabInc + arg0.getArg() + " = " + arg1.getArg() + " / pow(2," + arg2.getArg() + ");\n";
			}else {
				size_t nbChannel = arg0.mListChannel.size();
				for (size_t j = 0; j < nbChannel; j++)
					mHlslCode += tabInc + arg0.getArg_ForChannel(j) + " = " + arg1.getArg_ForChannel(j) + " / pow(2," + arg2.getArg_ForChannel(j) + ");\n";
			}
			registreResult.push_back(sv2.at(0));


		}else if ((instruction == "sincos") && (nbArg == 3)) {									// https://msdn.microsoft.com/en-us/library/windows/desktop/hh447234(v=vs.85).aspx


			Shader_AsmArg arg0(sv2.at(0), getShaderCBuffer_Var(sv2.at(0)));
			Shader_AsmArg arg1(sv2.at(1), getShaderCBuffer_Var(sv2.at(1)));
			Shader_AsmArg arg2(sv2.at(2), getShaderCBuffer_Var(sv2.at(2)));

			if (arg0.haveNoChannel())
			{
				mHlslCode += tabInc + arg0.getArg() + " = sin(" + arg2.getArg() + ");\n";
			}
			else {
				size_t nbChannel = arg0.mListChannel.size();
				for (size_t j = 0; j < nbChannel; j++)
					mHlslCode += tabInc + arg0.getArg_ForChannel(j) + " = sin(" + arg2.getArg_ForChannel(j) + ");\n";
			}
			registreResult.push_back(sv2.at(0));


			if (arg1.haveNoChannel())
			{
				mHlslCode += tabInc + arg1.getArg() + " = cos(" + arg2.getArg() + ");\n";
			}
			else {
				size_t nbChannel = arg1.mListChannel.size();
				for (size_t j = 0; j < nbChannel; j++)
					mHlslCode += tabInc + arg1.getArg_ForChannel(j) + " = cos(" + arg2.getArg_ForChannel(j) + ");\n";
			}
			registreResult.push_back(sv2.at(1));
			

		}else if ((instruction == "discard") && (nbArg == 1)) {									// https://msdn.microsoft.com/en-us/library/windows/desktop/hh446968(v=vs.85).aspx


			Shader_AsmArg arg0(sv2.at(0), getShaderCBuffer_Var(sv2.at(0)));

			if (arg0.haveNoChannel())
			{
				mHlslCode += tabInc + "if(" + arg0.getArg() + " != 0.0) clip(-1);\n";
			}
			else {
				size_t nbChannel = arg0.mListChannel.size();
				for (size_t j = 0; j < nbChannel; j++)
					mHlslCode += tabInc + "if(" + arg0.getArg_ForChannel(j) + " != 0.0) clip(-1);\n";
			}








		//------ Samples
		}else if (instruction == "sample_indexable(texture2d)(float,float,float,float)") {	// https://msdn.microsoft.com/en-us/library/windows/desktop/hh447230(v=vs.85).aspx
			
			
			string ouputChannel = rectifyChannels(sv2.at(0));
			bool haveToForce = (ouputChannel.length() > 0);					//the 0 (instead of 1) is for the problematic case : //r1.w = tex2D(Texture_ImageSampler2,v1.xy).yzwx; where ".yzw" is superfluous.

			Shader_AsmArg arg1(sv2.at(1), getShaderCBuffer_Var(sv2.at(1)));
			Shader_AsmArg arg2(sv2.at(2), getShaderCBuffer_Var(sv2.at(2)));

			size_t nbSampler = mListShaderSampler.size();
			for (size_t k = 0; k < nbSampler; k++)							//replace the sampler by its name.
			{
				if (mListShaderSampler.at(k).mCodeName== sv2.at(3))
				{
					sv2.at(3) = mListShaderSampler.at(k).mName;
					break;
				}
			}


			mHlslCode += tabInc + sv2.at(0) + " = " + (haveToForce ? "(" : "") + ((isAPixelShader) ? "tex2D" : "tex2Dlod") +"("+ sv2.at(3) +","+ arg1.getArg(isAPixelShader ? 2 : 4) + ")"+ (arg2.haveNoChannel() ? "" : ("."+ arg2.getAllChannelString())) + (haveToForce ? (")." + ouputChannel) : "") + ";\n";
			registreResult.push_back(sv2.at(0));

		}else if (instruction == "sample_b_indexable(texture2d)(float,float,float,float)") {	// https://msdn.microsoft.com/en-us/library/windows/desktop/hh447225(v=vs.85).aspx

			string ouputChannel = rectifyChannels(sv2.at(0));
			bool haveToForce = (ouputChannel.length() > 0);

			Shader_AsmArg arg1(sv2.at(1), getShaderCBuffer_Var(sv2.at(1)));
			Shader_AsmArg arg2(sv2.at(2), getShaderCBuffer_Var(sv2.at(2)));

			size_t nbSampler = mListShaderSampler.size();
			for (size_t k = 0; k < nbSampler; k++)
			{
				if (mListShaderSampler.at(k).mCodeName == sv2.at(3))
				{
					sv2.at(3) = mListShaderSampler.at(k).mName;
					break;
				}
			}
			mHlslCode += tabInc + sv2.at(0) + " = " + (haveToForce ? "(" : "") + ((isAPixelShader) ? "tex2D" : "tex2Dlod") + "(" + sv2.at(3) + "," + arg1.getArg(isAPixelShader ? 2 : 4) + ")" + (arg2.haveNoChannel() ? "" : ("." + arg2.getAllChannelString())) + (haveToForce ? (")." + ouputChannel) : "") + "; //not using LODBias\n";
			registreResult.push_back(sv2.at(0));

		}else if (instruction == "sample_c_indexable(texture2d)(float,float,float,float)") {	// https://msdn.microsoft.com/en-us/library/windows/desktop/hh447227(v=vs.85).aspx

			Shader_AsmArg arg1(sv2.at(1), getShaderCBuffer_Var(sv2.at(1)));

			size_t nbSampler = mListShaderSampler.size();
			for (size_t k = 0; k < nbSampler; k++)
			{
				if (mListShaderSampler.at(k).mCodeName == sv2.at(3))
				{
					sv2.at(3) = mListShaderSampler.at(k).mName;
					break;
				}
			}
			mHlslCode += tabInc + sv2.at(0) + " = " + ((isAPixelShader) ? "tex2D" : "tex2Dlod") + "(" + sv2.at(3) + "," + arg1.getArg(isAPixelShader ? 2 : 4) + ").r;\n";
			registreResult.push_back(sv2.at(0));

		}else if (instruction == "sample_l_indexable(texture2d)(float,float,float,float)") {	// https://msdn.microsoft.com/en-us/library/windows/desktop/hh447229(v=vs.85).aspx

			Shader_AsmArg arg1(sv2.at(1), getShaderCBuffer_Var(sv2.at(1)));

			size_t nbSampler = mListShaderSampler.size();
			for (size_t k = 0; k < nbSampler; k++)
			{
				if (mListShaderSampler.at(k).mCodeName == sv2.at(3))
				{
					sv2.at(3) = mListShaderSampler.at(k).mName;
					break;
				}
			}
			mHlslCode += tabInc + sv2.at(0) + " = " + ((isAPixelShader) ? "tex2D" : "tex2Dlod") + "(" + sv2.at(3) + "," + arg1.getArg(isAPixelShader ? 2 : 4) + ").r;\n";
			registreResult.push_back(sv2.at(0));



		}else if (instruction == "sample_indexable(texturecube)(float,float,float,float)") {


			string ouputChannel = rectifyChannels(sv2.at(0));
			bool haveToForce = (ouputChannel.length() > 0);
			Shader_AsmArg arg2(sv2.at(2), getShaderCBuffer_Var(sv2.at(2)));

			size_t nbSampler = mListShaderSampler.size();
			for (size_t k = 0; k < nbSampler; k++)
			{
				if (mListShaderSampler.at(k).mCodeName == sv2.at(3))
				{
					sv2.at(3) = mListShaderSampler.at(k).mName;
					break;
				}
			}
			mHlslCode += tabInc + sv2.at(0) + " = " + (haveToForce ? "(" : "") + " texCUBE(" + sv2.at(3) + "," + sv2.at(1) + ")" + (arg2.haveNoChannel() ? "" : ("." + arg2.getAllChannelString())) + (haveToForce ? (")." + ouputChannel) : "") + ";\n";
			registreResult.push_back(sv2.at(0));

		}else if (instruction == "sample_c_indexable(texturecube)(float,float,float,float)") {	// https://msdn.microsoft.com/en-us/library/windows/desktop/hh447227(v=vs.85).aspx

			size_t nbSampler = mListShaderSampler.size();
			for (size_t k = 0; k < nbSampler; k++)
			{
				if (mListShaderSampler.at(k).mCodeName == sv2.at(3))
				{
					sv2.at(3) = mListShaderSampler.at(k).mName;
					break;
				}
			}
			mHlslCode += tabInc + sv2.at(0) + " = texCUBE(" + sv2.at(3) + "," + sv2.at(1) + ").r;\n";
			registreResult.push_back(sv2.at(0));
		
		}else if (instruction == "ld_indexable(texture2d)(float,float,float,float)") {	// https://msdn.microsoft.com/en-us/library/windows/desktop/hh447172(v=vs.85).aspx

			string ouputChannel = rectifyChannels(sv2.at(0));
			bool haveToForce = (ouputChannel.length() > 0);

			Shader_AsmArg arg1(sv2.at(1), getShaderCBuffer_Var(sv2.at(1)));
			Shader_AsmArg arg2(sv2.at(2), getShaderCBuffer_Var(sv2.at(2)));

			size_t nbSampler = mListShaderSampler.size();
			
			if (sv2.at(2).substr(0, 1) == "t")
			{
				sv3 = split(sv2.at(2).substr(1), '.');
				
				std::string::size_type sz;
				size_t indexTexture = std::stoi(sv3.at(0).c_str(), &sz);

				if (indexTexture < nbSampler)
					sv2.at(2) = mListShaderSampler.at(indexTexture).mName;
			}

			for (size_t k = 0; k < nbSampler; k++)
			{
				if (mListShaderSampler.at(k).mCodeName == sv2.at(2))
				{
					sv2.at(2) = mListShaderSampler.at(k).mName;
					break;
				}
			}
			mHlslCode += tabInc + sv2.at(0) + " = " + (haveToForce ? "(" : "") + ((isAPixelShader) ? "tex2D" : "tex2Dlod") + "(" + sv2.at(2) + "," + arg1.getArg(isAPixelShader ? 2 : 4) + ")" + (arg2.haveNoChannel() ? "" : ("." + arg2.getAllChannelString())) + (haveToForce ? (")." + ouputChannel) : "") + ";\n";

			registreResult.push_back(sv2.at(0));


		}else if (instruction == "ld_structured_indexable(structured_buffer, stride=80)(mixed,mixed,mixed,mixed)") {	// https://msdn.microsoft.com/en-us/library/windows/desktop/hh447172(v=vs.85).aspx

			string ouputChannel = rectifyChannels(sv2.at(0));
			bool haveToForce = (ouputChannel.length() > 0);

			Shader_AsmArg arg1(sv2.at(1), getShaderCBuffer_Var(sv2.at(1)));
			Shader_AsmArg arg2(sv2.at(2), getShaderCBuffer_Var(sv2.at(2)));

			size_t nbSampler = mListShaderSampler.size();

			if (sv2.at(2).substr(0, 1) == "t")
			{
				sv3 = split(sv2.at(2).substr(1), '.');

				std::string::size_type sz;
				size_t indexTexture = std::stoi(sv3.at(0).c_str(), &sz);

				if (indexTexture < nbSampler)
					sv2.at(2) = mListShaderSampler.at(indexTexture).mName;
			}

			for (size_t k = 0; k < nbSampler; k++)
			{
				if (mListShaderSampler.at(k).mCodeName == sv2.at(2))
				{
					sv2.at(2) = mListShaderSampler.at(k).mName;
					break;
				}
			}
			mHlslCode += tabInc + sv2.at(0) + " = " + (haveToForce ? "(" : "") + ((isAPixelShader) ? "tex2D" : "tex2Dlod") + "(" + sv2.at(2) + "," + arg1.getArg(isAPixelShader ? 2 : 4) + ")" + (arg2.haveNoChannel() ? "" : ("." + arg2.getAllChannelString())) + (haveToForce ? (")." + ouputChannel) : "") + ";\n";
			registreResult.push_back(sv2.at(0));

			

		}else if (instruction == "resinfo_indexable(texture2d)(float,float,float,float)") {	// https://msdn.microsoft.com/en-us/library/windows/desktop/hh447214(v=vs.85).aspx

			mHlslCode += tabInc + sv2.at(0) + " = float4( max( 256.0 / pow(2.0, "+ sv2.at(1) +"), 1.0), max( 256.0 / pow(2.0, " + sv2.at(1) + "), 1.0), 1.0, 0.0);  // Not Possible into Directx9 shaderModel 3.0 to have width, height, depth or array size, total-mip-count of a texture. TODO change values by hands\n";
			registreResult.push_back(sv2.at(0));






		//------ Unknow Instructions.
		}else if(i+1!= nbLines){				//at the end of the file , it could have random chars. So only search wrong instructions from NotTheLastLine.
			
			printf("Could not Read instruction *%s* with argument %s on %s\n", instruction.c_str(), argumentsLine.c_str(), mName.c_str());
			//getchar();
		}


		//application of Modifiers on the result
		if ((useAbs) || (useSaturate))
		{
			size_t nbRegRes = registreResult.size();
			for (size_t k = 0; k < nbRegRes; k++)
			{
				if (useAbs)
					mHlslCode += tabInc + registreResult.at(k) + " = abs(" + registreResult.at(k) + ");\n";
				if (useSaturate)
					mHlslCode += tabInc + registreResult.at(k) + " = saturate(" + registreResult.at(k) + ");\n";
			}
		}



		//gestion of registre's declarations  (witch is done after, and it will be replace in hlslcode)
		size_t nbRegRes = registreResult.size();
		for (size_t k = 0; k < nbRegRes; k++)
		{
			sv3 = split(registreResult.at(k), '.');

			if (sv3.size() == 1)
				sv3.push_back("xyzw");
			
			std::string &registrerName = sv3.at(0);
			if (registrerName.substr(0, 1) == "o")					//that a Ouput, allready take care.
				continue;											//so do nothing.

			size_t isFound = (size_t)-1;
			size_t nbReg = mListShaderRegistrer.size();
			for (size_t j = 0; j < nbReg; j++)
			{
				if (mListShaderRegistrer.at(j).mName == registrerName)
				{
					isFound = j;
					break;
				}
			}
			if (isFound == (size_t)-1)
			{
				isFound = mListShaderRegistrer.size();
				mListShaderRegistrer.push_back(ShaderRegistrer(registrerName));
			}

			mListShaderRegistrer.at(isFound).addChannels(sv3.at(1));
		}

	}

	


	//ajust Input declarations
	std::string inputDeclaration = "\n";
	size_t nbInput = mListShaderRegistrer_VertexInput.size();
	for (size_t j = 0; j < nbInput; j++)
		inputDeclaration += "\t\t\t\t\t"+ mListShaderRegistrer_VertexInput.at(j).getFinalDeclaration(true) +",\n";

	//same for Ouput declarations (Notice, last don't have comma)
	size_t nbOutput = mListShaderRegistrer_Ouput.size();
	for (size_t j = 0; j < nbOutput; j++)
	{
		inputDeclaration += "\t\t\t\t\t\tout\t" + mListShaderRegistrer_Ouput.at(j).getFinalDeclaration(true, true) + ((j + 1 != nbOutput) ? "," : "") + "\n";


		if (mListShaderRegistrer_Ouput.at(j).mNbInputDeclaration != 1)
		{
			ShaderRegistrer master(mListShaderRegistrer_Ouput.at(j).mName);
			master.addChannels(mListShaderRegistrer_Ouput.at(j).getChannelOrdered());
			
			mHlslCode += tabInc + mListShaderRegistrer_Ouput.at(j).mName + "_0" + " = " + master.mName +"."+ mListShaderRegistrer_Ouput.at(j).getChannelOrdered() + ";\n";

			for (size_t k=0;k<nbOutput;k++)
			{
				size_t pos = mListShaderRegistrer_Ouput.at(k).mName.find('_');
				if ((pos == string::npos) || (mListShaderRegistrer_Ouput.at(k).mName.substr(0, pos) != master.mName))
					continue;
				master.addChannels(mListShaderRegistrer_Ouput.at(k).getChannelOrdered());

				mHlslCode += tabInc + mListShaderRegistrer_Ouput.at(k).mName + " = " + master.mName + "." + mListShaderRegistrer_Ouput.at(k).getChannelOrdered() + ";\n";
			}

			mHlslCode = master.getFinalDeclaration(false) + mHlslCode;
		}

		
	}

	mHlslCode += tabInc + "return;\n";
	tabInc = tabInc.substr(0, tabInc.length() - 1);
	mHlslCode += tabInc + "}\n";

	

	//declaration for uniform
	std::string uniform = "\n\n\n";
	size_t nbUniform = mListShaderCBuffer.size();
	size_t cBufferRegistreIndex = 0;
	size_t boolRegistreIndex = 0;
	for (size_t j = 0; j < nbUniform; j++)
	{
		ShaderCBuffer &cbuf = mListShaderCBuffer.at(j);
		size_t nbVar = cbuf.mListShaderCBuffer_Var.size();
		for (size_t k = 0; k < nbVar; k++)
		{
			ShaderCBuffer_Var &var = cbuf.mListShaderCBuffer_Var.at(k);

			if (var.mType == "bool")
			{
				uniform += "uniform bool\t\t" + var.mName + ((var.mIsArray) ? ("[" + std::to_string(var.mNbArrayElements) + "]") : "") + "\t\t\t\t\t:\tregister(b" + std::to_string(boolRegistreIndex) +");\n";
				boolRegistreIndex += ((var.mIsArray) ? (var.mNbArrayElements) : 1);

			}else if((var.mType == "float4") || (var.mType == "float3") || (var.mType == "float2") || (var.mType == "float") || (var.mType == "int")) {

				uniform += "uniform "+ var.mType +"\t\t" + var.mName + ((var.mIsArray) ? ("[" + std::to_string(var.mNbArrayElements) + "]") : "") + "\t\t\t\t\t:\tregister(c" + std::to_string(cBufferRegistreIndex) + ");\n";
				cBufferRegistreIndex += ((var.mIsArray) ? (var.mNbArrayElements) : 1);

			}else if (var.mType == "float4x4"){

				uniform += "uniform " + var.mType + "\t\t" + var.mName + ((var.mIsArray) ? ("[" + std::to_string(var.mNbArrayElements) + "]") : "") + "\t\t\t\t\t:\tregister(c" + std::to_string(cBufferRegistreIndex) + ");\n";
				cBufferRegistreIndex += ((var.mIsArray) ? (var.mNbArrayElements) : 1) * 4;

			}else if (var.mType == "float4x3") {

				uniform += "uniform " + var.mType + "\t\t" + var.mName + ((var.mIsArray) ? ("["+ std::to_string(var.mNbArrayElements) +"]") : "") + "\t\t\t\t\t:\tregister(c" + std::to_string(cBufferRegistreIndex) + ");\n";
				cBufferRegistreIndex += ((var.mIsArray) ? (var.mNbArrayElements) : 1) * 3;

			}else{

				printf("Unknown shader params type %s for %s in %s\n", var.mType.c_str(), var.mName.c_str(), mName.c_str());
				getchar();
			}

		}
		uniform += "\n";
	}
	//same for samplers
	size_t nbSampler = mListShaderSampler.size();
	for (size_t k = 0; k < nbSampler; k++)
	{
		ShaderSampler &sampler = mListShaderSampler.at(k);
		uniform += "uniform "+ std::string( (sampler.mType!="2d") ? "samplerCUBE" : "sampler2D") +"\t\t"+ sampler.mName + "\t\t\t\t\t:\tregister(" + sampler.mCodeName + ");\n";
	}	
	uniform += "\n\n\n";
	
	




	//ajust register declarations.
	std::string registerDeclaration = "";
	size_t nbReg = mListShaderRegistrer.size();
	for (size_t j = 0; j < nbReg; j++)
		registerDeclaration += mListShaderRegistrer.at(j).getFinalDeclaration();



	std::string previousFunctions = tabInc + "//////////////////////////////////////  Bit-wise function (missing in shader v3.0) //////////////////////////////////////////\n" +
									tabInc + "int slidetoLeft(int value, int nbMoves) { return value * pow(2.0, nbMoves); }						// << \n" +
									tabInc + "int slidetoRight(int value, int nbMoves) { return floor(value / pow(2.0, nbMoves)); }				// >>\n" +
									tabInc + "int getBit(int value, int bitIndex)										//index from right\n" +
									tabInc + "{\n" +
									tabInc + "	float base = floor(((float)value) / ((float)pow(2, bitIndex)));				//remove next right bits\n" +
									tabInc + "	return ((base - floor(base / 2.0) * 2.0) != 0.0) ? 1 : 0;				//remove previous left bits, and see if it's rest something.\n" +
									tabInc + "}\n" +
									tabInc + "float specialCheck_Dbxv2Shaders(float value)\n" +
									tabInc + "{\n" +
									tabInc + "	if (value == (int)0x3F800000) return 1.0;									//common case. and with precision problem (because under it's all float32 values), simplification is better.\n" +
									tabInc + "	if (value == (int)0x3E4CCCCD) return 0.2;\n" +
									tabInc + "	if (value == (int)0xC0490FDB) return -3.14159274;\n" +
									tabInc + "	if (value == (int)0x80000000) return 0.0;\n" +
									tabInc + "	return value;\n" +
									tabInc + "}\n" +
									tabInc + "float and_BitWise_operation(float a, float b)									//notice it's on 32 bits\n" +
									tabInc + "{\n" +
									tabInc + "	if (a == (int)0xFFFFFFFF) return b;										//common case. and with precision problem (because under it's all float32 values), simplification is better.\n" +
									tabInc + "	if (b == (int)0xFFFFFFFF) return a;\n" +
									tabInc + "	if (b == a) return a;\n" +
									tabInc + "	float c = 0;\n" +
									tabInc + "	for (int i = 0; i < 32; i++)\n" +
									tabInc + "		c += (getBit(floor(a), i) && getBit(floor(b), i)) * pow(2, i);\n" +
									tabInc + "	return c;\n" +
									tabInc + "}\n" +
									tabInc + "float or_BitWise_operation(float a, float b)									//notice it's on 32 bits\n" +
									tabInc + "{\n" +
									tabInc + "	if (a == (int)0xFFFFFFFF) return a;										//common case. and with precision problem (because under it's all float32 values), simplification is better.\n" +
									tabInc + "	if (b == (int)0xFFFFFFFF) return b;\n" +
									tabInc + "	if (b == a) return a;\n" +
									tabInc + "	float c = 0;\n" +
									tabInc + "	for (int i = 0; i < 32; i++)\n" +
									tabInc + "		c += (getBit(floor(a), i) || getBit(floor(b), i)) * pow(2, i);\n" +
									tabInc + "	return c;\n" +
									tabInc + "}\n" +
									tabInc + "/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n" +
									tabInc + "\n";

	

	mHlslCode = uniform + previousFunctions +"void\tmain_"+ ((isAPixelShader) ? "ps" : "vs")  +"("+ inputDeclaration +"\t\t\t\t\t)\n{\n"+ registerDeclaration  + mHlslCode;

	

}
/*-------------------------------------------------------------------------------\
|                             getShaderCBuffer_Var				                 |
\-------------------------------------------------------------------------------*/
Shader_vs_5_0::ShaderCBuffer_Var* Shader_vs_5_0::getShaderCBuffer_Var(const string &name)
{
	size_t nbCbuffer = mListShaderCBuffer.size();
	for (size_t i = 0; i < nbCbuffer; i++)
	{
		std::vector<ShaderCBuffer_Var> &listVar = mListShaderCBuffer.at(i).mListShaderCBuffer_Var;

		size_t nbVar = listVar.size();
		for (size_t j = 0; j < nbVar; j++)
		{
			if (listVar.at(j).mName == name)
				return &(listVar.at(j));
		}
	}

	return 0;
}

/*-------------------------------------------------------------------------------\
|                             rectifyChannels									 |
\-------------------------------------------------------------------------------*/
string Shader_vs_5_0::rectifyChannels(const string &ouputArg)
{
	Shader_AsmArg argOut(ouputArg);
	
	if (argOut.haveNoChannel())
		return "";

	size_t nbChannels = argOut.mListChannel.size();
	ShaderInstructionChannel nextValue = ShaderInstructionChannel::SIC_x;
	bool haveContinuity = true;
	for (size_t i = 0; i < nbChannels; i++)
	{
		//test of channel continuity
		if (argOut.mListChannel.at(i) != nextValue)
		{
			haveContinuity = false;
			break;
		}
		nextValue = (ShaderInstructionChannel)( ((int)nextValue) * 2);
	}
	
	if(haveContinuity)
		return "";

	return argOut.getAllChannelString();
}



/*-------------------------------------------------------------------------------\
|                             parse					                             |
\-------------------------------------------------------------------------------*
void Shader_vs_5_0::ShaderOperation::parse(std::string str)
{

}
/*-------------------------------------------------------------------------------\
|                             writeHlsl				                             |
\-------------------------------------------------------------------------------*/
std::string Shader_vs_5_0::writeHlsl(void)
{
	return mHlslCode;
}

}