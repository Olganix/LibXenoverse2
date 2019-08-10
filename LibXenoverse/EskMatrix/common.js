
/**
 * Copyright (c) Olganix; Licensed
 */

//String.toLowerCase(), String.toUpperCase()




////////////////////////////////////////////////// Divers

function ckv(variable) { return ((variable != undefined)&&(variable != null)); }               // check if a variable exist and is not null (to reduce display in code)


function downloadTextFile(text, filename)
{
    if(!ckv(filename))
        filename = "download.txt";

    if((is_safari())&&(is_ios()))                     //safari ios , and chrome ios (witch is on safari technologie ... pff)
    {
        window.open('data:application/json;charset=utf-8,' + encodeURIComponent( text ), '_blank');
        return;
    }

    var isFirefox = is_fireFox();

    var download = document.createElement('a');
    download.setAttribute('href', "data:text/plain;charset=utf-8," + encodeURIComponent( text ));
    download.setAttribute('download', filename);
    
    if(isFirefox)                                           //required in FF
    {
        download.target="_self" ;                           
        download.style = "visibility:hidden"; 
        document.body.appendChild(download);
    }
    
    download.click();

    if(isFirefox)
        document.body.removeChild(download); 
}

function downloadBlobUrlFile(blobURL, filename)
{
    if(!ckv(filename))
        filename = "download.txt";

    var isFirefox = is_fireFox();

    var download = document.createElement('a');
    download.setAttribute('href', blobURL);
    download.setAttribute('download', filename);
    
    if(isFirefox)                                           //required in FF
    {
        download.target="_self" ;                           
        download.style = "visibility:hidden"; 
        document.body.appendChild(download);
    }
    
    download.click();

    if(isFirefox)
        document.body.removeChild(download); 
}

////////////////////////////////////////////////// Number

function isInt(n) { return Number(n) === n && n % 1 === 0; }
function isFloat(n) { return Number(n) === n && n % 1 !== 0; }
function isNumber(n) { return Number(n) === n; }

function isInt_b(n) { return Number(n) == n && n % 1 === 0; }           //todo check why === in previous
function isFloat_b(n) { return Number(n) == n && n % 1 !== 0; }
function isNumber_b(n) { return Number(n) == n; }

function numberTostring(value, nbChiffre, charToLeftFill)           //left Fill
{
    if(!ckv(charToLeftFill))
        charToLeftFill = "0";

    var str = value.toString();
    var len = str.length;

    for (var i = len; i < nbChiffre; i++)
        str = charToLeftFill + str;

    return str;
}

////////////////////////////////////////////////// String

String.prototype.replaceAt=function(index, replacement)
{
    return this.substr(0, index) + replacement+ this.substr(index + replacement.length);
}

String.prototype.replaceAll_regEx = function(search, replacement)           //https://stackoverflow.com/questions/1144783/how-to-replace-all-occurrences-of-a-string-in-javascript
{
    var target = this;
    return target.replace(new RegExp(search, 'g'), replacement);
};

String.prototype.replaceAll = function(search, replacement)
{
    var target = this;
    return target.split(search).join(replacement);
};

String.prototype.replaceForHtml = function(alsoEscape)
{
    var target = this;
    target = target.replace(/&/g, "&amp;").replace(/>/g, "&gt;").replace(/</g, "&lt;").replace(/"/g, "&quot;");

    if(alsoEscape)
        target = target.replaceAll("\n", "<br>");

    return target;
};
String.prototype.replaceFromHtml = function(alsoEscape)
{
    var target = this;
    target = target.replace("&amp;", "&").replace("&gt;", ">").replace("&lt;", "<").replace("&quot;", '"');

    if(alsoEscape)
        target = target.replaceAll("<br>", "\n");

    return target;
};





String.prototype.detectCommonRanges_lines = function(str)
{
    var strA = this;

    var linesA = strA.split("\n");
    var linesB = strA.split("\n");

    var nbLinesA = linesA.length;
    var nbLinesB = linesB.length;

    var ranges = new Array();


    var range = {start: 0, startOther: 0, number: 0};

    var startB = 0;                                 //it's the start of linesB after a match of line.
    for(var i=0;i<nbLinesA;i++)                     //the range is refere to this (A) linesIndex, not B
    {
        var lineA = linesA[i];

        var thereIsAMatch = false;
        for(var j=startB;j<nbLinesB;j++)
        {
            if(lineA==linesB[j])
            {
                if(j==startB)                       //if it's continue, so the same range,
                {
                    range.number += 1;              //we just increment everyones

                }else{                              //if there is a gap to match (in linesB) a lines, we start a new range
                    ranges.push(range);             //so we save the last one
                    range = {start: i, startOther: j, number: 1};   //and begin a new one.
                }

                thereIsAMatch = true;
                startB = j + 1;
                break;
            }
        }

        if(!thereIsAMatch)                          //if no line match with the lineA,
        {
            ranges.push(range);                     //so we save the last one range
            range = {start: (i+1), startOther: startB, number: 0};   //and begin a new one.
        }
    }
    
    if((range.start<nbLinesA)&&(range.number!=0))   //save the last range.
        ranges.push(range);


    //now we keep only range with more than 0 matchs
    var ranges_cleaned = new Array();
    var nbRanges = ranges.length;
    for(var i=0;i<nbRanges;i++)
        if(ranges[i].number!=0)
            ranges_cleaned.push(ranges[i]);
    
    
    return ranges_cleaned;
};


////////////////////////////////////////////////// Object

function lengthOf(myObj)
{
    return Object.keys(myObj).length;               //version witch don't care about hasOwnProperty (cf https://stackoverflow.com/questions/5223/length-of-a-javascript-object )
}
function keyAt(myObj, index)
{
    if(index>=Object.keys(myObj).length)
        return null;

    return Object.keys(myObj)[index];
}
function objectAt(myObj, index)
{
    if(index>=Object.keys(myObj).length)
        return null;

    return myObj[ Object.keys(myObj)[index] ];
}


function iterationCopy(src)
{
    var target = {};
    for (var prop in src) 
        if (src.hasOwnProperty(prop))
            target[prop] = src[prop];
    return target;
}


////////////////////////////////////////////////// Array/List

function getRandomIndexList(nbElements)
{
    var ret = new Array();
    if(nbElements<=0)
        return ret;

    var listIndexNotUsed = new Array();
    for(var i=0;i<nbElements;i++)
        listIndexNotUsed.push(i);

    while(listIndexNotUsed.length > 0)
    {
        var notUsedIndex = Math.round(Math.random() * (listIndexNotUsed.length-1) );
        ret.push( listIndexNotUsed[notUsedIndex] );

        listIndexNotUsed.splice(notUsedIndex, 1);
    }

    return ret;
}


////////////////////////////////////////////////// Time

function timeFormat(time, displayMs)
{
    if((displayMs==undefined)||(displayMs==null))
        displayMs = false;

    var minutes = Math.floor(time/60.0);
    var second = Math.floor(time - minutes * 60.0);
    var ms = Math.floor(((time - minutes * 60.0) - second) * 1000);

    return minutes.toString() +"m "+ numberTostring(second, 2) +"s " + (displayMs ? (numberTostring(ms.toString(),3) +"ms ") : "");
}

function timeFormat_data(time)
{
    var minutes = Math.floor(time/60.0);
    var second = Math.floor(time - minutes * 60.0);
    var ms = Math.floor(((time - minutes * 60.0) - second) * 1000);

    return {"min": minutes, "sec": second, "ms" : ms};
}

//notice, you could also use moment.js lib.


////////////////////////////////////////////////// Math

var radTodeg = (180.0 / Math.PI);
var degTorad = (Math.PI / 180.0);

function modulo(value, modulo)
{
    return (value - Math.floor(value / modulo) * modulo);
}


function lerp(valueA, valueB, factor)
{
    if((typeof valueA == "object")&&(Array.isArray(valueA))&&(typeof valueB == "object")&&(Array.isArray(valueB)))
    {
        var nbA = valueA.length;
        var nbB = valueB.length;
        var nbElements = nbA >= nbB ? nbA : nbB;

        var ret = new Array();
        for(var i=0;i<nbElements;i++)
            ret.push( ((i<nbA) ? valueA[i] : 0) * (1-factor) + ((i<nbB) ? valueB[i] : 0) * factor );
    
        return ret;
    }else{
        return valueA * (1-factor) + valueB * factor;
    }
}


function saturate(value, minValue, maxValue)
{
    if(!ckv(minValue))
        minValue = 0;
    if(!ckv(maxValue))
        maxValue = 1;
    
    if((typeof value == "object")&&(Array.isArray(value)))
    {
        var ret = new Array();

        var nbElements = value.length;
        for(var i=0;i<nbElements;i++)
        {
            var val = value[i];
            if(val < minValue) val = minValue;
            if(val > maxValue) val = maxValue;

            ret.push( val );
        }
        return ret;
    }else{
        
        if(value < minValue) value = minValue;
        if(value > maxValue) value = maxValue;

        return value;
    }
}




function cubic_bezier(x, a, b, c, d)
{
    if(typeof a == "string")
    {
        switch(a)
        {
            case "linear": return cubic_bezier(x, 0, 0, 1, 1); break;
            case "ease": return cubic_bezier(x, 0.25, 0.1, 0.25, 1); break;       //to follow some animations   //todo test all case https://css-tricks.com/ease-out-in-ease-in-out/ , https://www.alsacreations.com/tuto/lire/876-transitions-css3-transition-timing-function.html
            case "ease-in": return cubic_bezier(x, 0.42, 0, 1, 1); break; 
            case "ease-out": return cubic_bezier(x, 0, 0, 0.58, 1);  break;
            case "ease-in-out": return cubic_bezier(x, 0.42, 0, 0.58, 1);  break;
        };
    }

    return (a * Math.pow(1 - x, 3)) + (3 * b * Math.pow(1 - x, 2) * x) + (3 * c * (1 - x) * Math.pow(x, 2)) + (d * Math.pow(x, 3));
}
function lerp_cubic_bezier(start, end, factor, a, b, c, d)
{
    return start + (end - start) * cubic_bezier(factor, a, b, c, d);
}




/*******************************************************************************
*                                   calcul_TransformFromMatrix4x4              *
*******************************************************************************/
function calcul_Dbvx_TransformFromMatrix4x4(values)
{
    if(values.length!=16)
        return {errors_str: "Error: not enought values for Matrix4x4, 16 expected"};
    
    var inversedMatrixValues = calcul_Matrix_inverse4x4(values);                  // Dbxv2 way, strange but it's true (used in xenoviewer and emdfbx.)
    if(ckv(inversedMatrixValues.errors_str))
        return inversedMatrixValues;
    

    inversedMatrixValues = calcul_Matrix_transpose4x4(inversedMatrixValues);		//come back to Ogre matrice way (MAtrix convention)
    if(ckv(inversedMatrixValues.errors_str))
        return inversedMatrixValues;

    var transformValues = calcul_Matrix_decomposition4x4(inversedMatrixValues);
    if(ckv(transformValues.errors_str))
        return transformValues;

    return transformValues;
}
/*******************************************************************************
*                                   calcul_Dbvx_Matrix4x4FromTransform         *
*******************************************************************************/
function calcul_Dbvx_Matrix4x4FromTransform(values)
{
    if(values.length!=12)
        return {errors_str: "Error: not enought values for Transform, 3x4=12 expected"};
    
    var matrixValues = calcul_Matrix_makeTransform4x4(values);
    if(ckv(matrixValues.errors_str))
        return matrixValues;

    matrixValues = calcul_Matrix_transpose4x4(matrixValues);		//come back to Ogre matrice way (MAtrix convention)  //in Ogre, position is in last colone, but in Esk, position is on the last row, so we will use Transpose transformation.
    if(ckv(matrixValues.errors_str))
        return matrixValues;
    
    var inversedMatrixValues = calcul_Matrix_inverse4x4(matrixValues);                  // Dbxv2 way, strange but it's true (used in xenoviewer and emdfbx.)
    if(ckv(inversedMatrixValues.errors_str))
        return inversedMatrixValues;
    
    return inversedMatrixValues;
}
/*******************************************************************************
*                                   calcul_Dbvx_RealtiveMatrix4x4              *
*******************************************************************************/
function calcul_Dbvx_RelativeMatrix4x4(values, parentMatrixValues)
{
    if(values.length!=16)
        return {errors_str: "Error: not enought values for Matrix4x4, 16 expected"};
    
    var inversedParentMatrixValues = calcul_Matrix_inverse4x4(parentMatrixValues);			//inverse of parent transformation
    if(ckv(inversedParentMatrixValues.errors_str))
        return inversedParentMatrixValues;

    var matrix_tmp = calcul_Matrix_concatenate4x4(inversedParentMatrixValues, values);        // to revert parent transformation from absolute transformation of node.
    if(ckv(matrix_tmp.errors_str))
        return matrix_tmp;

    matrix_tmp = calcul_Matrix_inverse4x4(matrix_tmp);                            //Dbvx2 way
    if(ckv(matrix_tmp.errors_str))
        return matrix_tmp;

    var relativeMatrix = calcul_Matrix_transpose4x4(matrix_tmp);		                    //come back to Ogre matrice way
    if(ckv(relativeMatrix.errors_str))
        return relativeMatrix;

    return relativeMatrix;
}
/*******************************************************************************
*                                   calcul_Dbvx_RealtiveMatrix4x4              *
*******************************************************************************/
function calcul_Dbvx_AbsoluteMatrix4x4(values, parentMatrixValues)
{
    if(values.length!=16)
        return {errors_str: "Error: not enought values for Matrix4x4, 16 expected"};

    var matrix_tmp = calcul_Matrix_transpose4x4(values);                          //come back to Ogre matrice way
    if(ckv(matrix_tmp.errors_str))
        return matrix_tmp;

    matrix_tmp = calcul_Matrix_inverse4x4(matrix_tmp);                            //Dbvx2 way
    if(ckv(matrix_tmp.errors_str))
        return matrix_tmp;

    var absoluteMatrix = calcul_Matrix_concatenate4x4(parentMatrixValues, matrix_tmp);     //add parent absolute transform to the relative  transformation of node.
    if(ckv(absoluteMatrix.errors_str))
        return absoluteMatrix;

    return absoluteMatrix;
}



/*
//from Ogre Matrix4x4
m[0][0] = m00;	0   m[0][1] = m01;	1   m[0][2] = m02;	2   m[0][3] = m03;	3
m[1][0] = m10;	4   m[1][1] = m11;	5   m[1][2] = m12;	6   m[1][3] = m13;	7
m[2][0] = m20;	8   m[2][1] = m21;	9   m[2][2] = m22;	10  m[2][3] = m23;	11
m[3][0] = m30;	12  m[3][1] = m31;	13  m[3][2] = m32;	14  m[3][3] = m33;	15
*/

/*******************************************************************************
*                                   calcul_Matrix_transpose4x4                 *
*******************************************************************************/
function calcul_Matrix_transpose4x4(matrix)              //highly inspired from Ogre Engine
{
    var newMatrix = new Array();

	var m00 = matrix[0],  m01 = matrix[1],  m02 = matrix[2],  m03 = matrix[3];
	var m10 = matrix[4],  m11 = matrix[5],  m12 = matrix[6],  m13 = matrix[7];
	var m20 = matrix[8],  m21 = matrix[9],  m22 = matrix[10], m23 = matrix[11];
	var m30 = matrix[12], m31 = matrix[13], m32 = matrix[14], m33 = matrix[15];
		
	newMatrix.push(m00); newMatrix.push(m10); newMatrix.push(m20); newMatrix.push(m30);
	newMatrix.push(m01); newMatrix.push(m11); newMatrix.push(m21); newMatrix.push(m31);
	newMatrix.push(m02); newMatrix.push(m12); newMatrix.push(m22); newMatrix.push(m32);
	newMatrix.push(m03); newMatrix.push(m13); newMatrix.push(m23); newMatrix.push(m33);
    
    return newMatrix;
}
/*******************************************************************************
*                                   calcul_TransformFromMatrix4x4              *
*******************************************************************************/
function calcul_Matrix_inverse4x4(matrix)              //highly inspired from Ogre Engine
{
	var m00 = matrix[0],  m01 = matrix[1],  m02 = matrix[2],  m03 = matrix[3];
	var m10 = matrix[4],  m11 = matrix[5],  m12 = matrix[6],  m13 = matrix[7];
	var m20 = matrix[8],  m21 = matrix[9],  m22 = matrix[10], m23 = matrix[11];
	var m30 = matrix[12], m31 = matrix[13], m32 = matrix[14], m33 = matrix[15];

	var v0 = m20 * m31 - m21 * m30;
	var v1 = m20 * m32 - m22 * m30;
	var v2 = m20 * m33 - m23 * m30;
	var v3 = m21 * m32 - m22 * m31;
	var v4 = m21 * m33 - m23 * m31;
	var v5 = m22 * m33 - m23 * m32;

	var t00 = +(v5 * m11 - v4 * m12 + v3 * m13);
	var t10 = -(v5 * m10 - v2 * m12 + v1 * m13);
	var t20 = +(v4 * m10 - v2 * m11 + v0 * m13);
	var t30 = -(v3 * m10 - v1 * m11 + v0 * m12);

	var invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

	var d00 = t00 * invDet;
	var d10 = t10 * invDet;
	var d20 = t20 * invDet;
	var d30 = t30 * invDet;

	var d01 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	var d11 =  (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	var d21 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	var d31 =  (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

	v0 = m10 * m31 - m11 * m30;
	v1 = m10 * m32 - m12 * m30;
	v2 = m10 * m33 - m13 * m30;
	v3 = m11 * m32 - m12 * m31;
	v4 = m11 * m33 - m13 * m31;
	v5 = m12 * m33 - m13 * m32;

	var d02 =  (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	var d12 = -(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	var d22 =  (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	var d32 = -(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

	v0 = m21 * m10 - m20 * m11;
	v1 = m22 * m10 - m20 * m12;
	v2 = m23 * m10 - m20 * m13;
	v3 = m22 * m11 - m21 * m12;
	v4 = m23 * m11 - m21 * m13;
	v5 = m23 * m12 - m22 * m13;

	var d03 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	var d13 =  (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	var d23 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	var d33 =  (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

    var inversedMatrix = new Array();
	inversedMatrix.push(d00); inversedMatrix.push(d01); inversedMatrix.push(d02); inversedMatrix.push(d03);
    inversedMatrix.push(d10); inversedMatrix.push(d11); inversedMatrix.push(d12); inversedMatrix.push(d13); 
    inversedMatrix.push(d20); inversedMatrix.push(d21); inversedMatrix.push(d22); inversedMatrix.push(d23); 
    inversedMatrix.push(d30); inversedMatrix.push(d31); inversedMatrix.push(d32); inversedMatrix.push(d33);
    
    return inversedMatrix;
}
/*******************************************************************************
*                                   calcul_TransformFromMatrix4x4              *
*******************************************************************************/
function calcul_Matrix_concatenate4x4(matrixA, matrixB)              //highly inspired from Ogre Engine, equivalent at the multiplication of matrix.
{
	var m00 = matrixA[0],  m01 = matrixA[1],  m02 = matrixA[2],  m03 = matrixA[3];
	var m10 = matrixA[4],  m11 = matrixA[5],  m12 = matrixA[6],  m13 = matrixA[7];
	var m20 = matrixA[8],  m21 = matrixA[9],  m22 = matrixA[10], m23 = matrixA[11];
	var m30 = matrixA[12], m31 = matrixA[13], m32 = matrixA[14], m33 = matrixA[15];
		
	var m00_b = matrixB[0],  m01_b = matrixB[1],  m02_b = matrixB[2],  m03_b = matrixB[3];
	var m10_b = matrixB[4],  m11_b = matrixB[5],  m12_b = matrixB[6],  m13_b = matrixB[7];
	var m20_b = matrixB[8],  m21_b = matrixB[9],  m22_b = matrixB[10], m23_b = matrixB[11];
	var m30_b = matrixB[12], m31_b = matrixB[13], m32_b = matrixB[14], m33_b = matrixB[15];

	var m00_r = 0.0, m01_r = 0.0, m02_r = 0.0, m03_r = 0.0;
	var m10_r = 0.0, m11_r = 0.0, m12_r = 0.0, m13_r = 0.0;
	var m20_r = 0.0, m21_r = 0.0, m22_r = 0.0, m23_r = 0.0;
	var m30_r = 0.0, m31_r = 0.0, m32_r = 0.0, m33_r = 0.0;

	m00_r = m00 * m00_b + m01 * m10_b + m02 * m20_b + m03 * m30_b;
	m01_r = m00 * m01_b + m01 * m11_b + m02 * m21_b + m03 * m31_b;
	m02_r = m00 * m02_b + m01 * m12_b + m02 * m22_b + m03 * m32_b;
	m03_r = m00 * m03_b + m01 * m13_b + m02 * m23_b + m03 * m33_b;

	m10_r = m10 * m00_b + m11 * m10_b + m12 * m20_b + m13 * m30_b;
	m11_r = m10 * m01_b + m11 * m11_b + m12 * m21_b + m13 * m31_b;
	m12_r = m10 * m02_b + m11 * m12_b + m12 * m22_b + m13 * m32_b;
	m13_r = m10 * m03_b + m11 * m13_b + m12 * m23_b + m13 * m33_b;

	m20_r = m20 * m00_b + m21 * m10_b + m22 * m20_b + m23 * m30_b;
	m21_r = m20 * m01_b + m21 * m11_b + m22 * m21_b + m23 * m31_b;
	m22_r = m20 * m02_b + m21 * m12_b + m22 * m22_b + m23 * m32_b;
	m23_r = m20 * m03_b + m21 * m13_b + m22 * m23_b + m23 * m33_b;

	m30_r = m30 * m00_b + m31 * m10_b + m32 * m20_b + m33 * m30_b;
	m31_r = m30 * m01_b + m31 * m11_b + m32 * m21_b + m33 * m31_b;
	m32_r = m30 * m02_b + m31 * m12_b + m32 * m22_b + m33 * m32_b;
	m33_r = m30 * m03_b + m31 * m13_b + m32 * m23_b + m33 * m33_b;

    var matrix = new Array();
	matrix.push(m00_r); matrix.push(m01_r); matrix.push(m02_r); matrix.push(m03_r);
	matrix.push(m10_r); matrix.push(m11_r); matrix.push(m12_r); matrix.push(m13_r);
	matrix.push(m20_r); matrix.push(m21_r); matrix.push(m22_r); matrix.push(m23_r);
    matrix.push(m30_r); matrix.push(m31_r); matrix.push(m32_r); matrix.push(m33_r);
    return matrix;
}
/*******************************************************************************
*                                   calcul_Matrix_decomposition4x4             *
*******************************************************************************/
function calcul_Matrix_decomposition4x4(matrix)         //posOrientScaleMatrix is 3x4, orient is a quaternion informations, TransformMatrix is 4x4              //highly inspired from Ogre Engine
{
	var m00 = matrix[0],  m01 = matrix[1],  m02 = matrix[2],  m03 = matrix[3];
	var m10 = matrix[4],  m11 = matrix[5],  m12 = matrix[6],  m13 = matrix[7];
	var m20 = matrix[8],  m21 = matrix[9],  m22 = matrix[10], m23 = matrix[11];
	var m30 = matrix[12], m31 = matrix[13], m32 = matrix[14], m33 = matrix[15];

	if (!((Math.abs(m30) <= 0.000001) && (Math.abs(m31) <= 0.000001) && (Math.abs(m32) <= 0.000001) && (Math.abs(m33 - 1) <= 0.000001)))		//assert(isAffine());
        return {errors_str: "Error: Matrix4x4 is not afine, we can't convert into Transforms position rotation and scale."};

    //position
    var position = new Array();
	position.push(m03);
	position.push(m13);
    position.push(m23);
    position.push(m33);


	//Matrix3 matQ;
	//Vector3 vecU;
	//m3x3.QDUDecomposition(matQ, scale, vecU);
			
	// Factor M = QR = QDU where Q is orthogonal, D is diagonal,
	// and U is upper triangular with ones on its diagonal.  Algorithm uses
	// Gram-Schmidt orthogonalization (the QR algorithm).
	//
	// If M = [ m0 | m1 | m2 ] and Q = [ q0 | q1 | q2 ], then
	//
	//   q0 = m0/|m0|
	//   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
	//   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
	//
	// where |V| indicates length of vector V and A*B indicates dot
	// product of vectors A and B.  The matrix R has entries
	//
	//   r00 = q0*m0  r01 = q0*m1  r02 = q0*m2
	//   r10 = 0      r11 = q1*m1  r12 = q1*m2
	//   r20 = 0      r21 = 0      r22 = q2*m2
	//
	// so D = diag(r00,r11,r22) and U has entries u01 = r01/r00,
	// u02 = r02/r00, and u12 = r12/r11.

	// Q = rotation
	// D = scaling
	// U = shear

	// D stores the three diagonal entries r00, r11, r22
	// U stores the entries U[0] = u01, U[1] = u02, U[2] = u12


	// build orthogonal matrix Q
	var fInvLength = 1.0 / Math.sqrt( m00 * m00 + m10 * m10 + m20 * m20);
	var kQ_00 = m00 * fInvLength;
	var kQ_10 = m10 * fInvLength;
	var kQ_20 = m20 * fInvLength;

	var fDot = kQ_00 * m01 + kQ_10 * m11 + kQ_20 * m21;
	var kQ_01 = m01 - fDot * kQ_00;
	var kQ_11 = m11 - fDot * kQ_10;
	var kQ_21 = m21 - fDot * kQ_20;
	fInvLength = 1.0 / Math.sqrt( kQ_01 * kQ_01 + kQ_11 * kQ_11 + kQ_21 * kQ_21);
	kQ_01 *= fInvLength;
	kQ_11 *= fInvLength;
	kQ_21 *= fInvLength;

	fDot = kQ_00 * m02 + kQ_10 * m12 + kQ_20 * m22;
	var kQ_02 = m02 - fDot*kQ_00;
	var kQ_12 = m12 - fDot*kQ_10;
	var kQ_22 = m22 - fDot*kQ_20;
	fDot = kQ_01 * m02 + kQ_11 * m12 + kQ_21 * m22;
	kQ_02 -= fDot*kQ_01;
	kQ_12 -= fDot*kQ_11;
	kQ_22 -= fDot*kQ_21;
	fInvLength = 1.0 / Math.sqrt( kQ_02 * kQ_02 + kQ_12 * kQ_12 + kQ_22 * kQ_22);
	kQ_02 *= fInvLength;
	kQ_12 *= fInvLength;
	kQ_22 *= fInvLength;

	// guarantee that orthogonal matrix has determinant 1 (no reflections)
	var fDet = kQ_00 * kQ_11 * kQ_22 + kQ_01 * kQ_12 * kQ_20 + kQ_02 * kQ_10 * kQ_21 - kQ_02 * kQ_11 * kQ_20 - kQ_01 * kQ_10 * kQ_22 - kQ_00 * kQ_12 * kQ_21;

	if (fDet < 0.0)
	{
		kQ_00 = -kQ_00; kQ_01 = -kQ_01; kQ_02 = -kQ_02; kQ_10 = -kQ_10; kQ_11 = -kQ_11; kQ_12 = -kQ_12; kQ_20 = -kQ_20; kQ_21 = -kQ_21; kQ_22 = -kQ_22;
	}

	// build "right" matrix R
	var kR_00 = kQ_00 * m00 + kQ_10 * m10 + kQ_20 * m20;
	var kR_01 = kQ_00 * m01 + kQ_10 * m11 + kQ_20 * m21;
	var kR_11 = kQ_01 * m01 + kQ_11 * m11 + kQ_21 * m21;
	var kR_02 = kQ_00 * m02 + kQ_10 * m12 + kQ_20 * m22;
	var kR_12 = kQ_01 * m02 + kQ_11 * m12 + kQ_21 * m22;
	var kR_22 = kQ_02 * m02 + kQ_12 * m12 + kQ_22 * m22;

	// the scaling component
	var kD_0 = kR_00;
	var kD_1 = kR_11;
	var kD_2 = kR_22;

	// the shear component
	var fInvD0 = 1.0 / kD_0;
	var kU_0 = kR_01 * fInvD0;
	var kU_1 = kR_02 * fInvD0;
	var kU_2 = kR_12 / kD_1;


    var scale = new Array();
	scale.push(kD_0);
	scale.push(kD_1);
	scale.push(kD_2);
	scale.push(1.0);



    var orientation = new Array();
	//orientation = Quaternion(matQ);		//this->FromRotationMatrix(rot);
	// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
	// article "Quaternion Calculus and Fast Animation".

	var fTrace = kQ_00 + kQ_11 + kQ_22;
	var fRoot;

	if (fTrace > 0.0)
	{
		// |w| > 1/2, may as well choose w > 1/2
		fRoot = Math.sqrt(fTrace + 1.0);  // 2w
		var w = 0.5 * fRoot;					//w
		fRoot = 0.5 / fRoot;  // 1/(4w)
		orientation.push( (kQ_21 - kQ_12) * fRoot);
		orientation.push( (kQ_02 - kQ_20) * fRoot);
        orientation.push( (kQ_10 - kQ_01) * fRoot);
        orientation.push( w );
	}else{

		// |w| <= 1/2
		if (kQ_11 > kQ_00)
		{
			if (kQ_22 > kQ_11)
			{
				//i = 2;
				//size_t j = 0;
				//size_t k = 1;
				fRoot = Math.sqrt(kQ_22 - kQ_00 - kQ_11 + 1.0);
				var z = 0.5 * fRoot;					        //z
				fRoot = 0.5 / fRoot;
				orientation.push( (kQ_02 + kQ_20) * fRoot);		//x
                orientation.push( (kQ_12 + kQ_21) * fRoot);		//y
                orientation.push( z );
                orientation.push( (kQ_10 - kQ_01) * fRoot);		//w
			}else{
				//i = 1
				//size_t j = 2;
				//size_t k = 0;
				fRoot = Math.sqrt(kQ_11 - kQ_22 - kQ_00 + 1.0);
				var y = 0.5 * fRoot;							//y
				fRoot = 0.5 / fRoot;
                orientation.push( (kQ_01 + kQ_10) * fRoot);		//x
                orientation.push( y );
                orientation.push( (kQ_21 + kQ_12) * fRoot);		//z
                orientation.push( (kQ_02 - kQ_20) * fRoot);		//w
			}
		}else{

			if (kQ_22 > kQ_00)
			{
				//i = 2;
				//size_t j = 0;
				//size_t k = 1;
				fRoot = Math.sqrt(kQ_22 - kQ_00 - kQ_11 + 1.0);
				var z = 0.5 * fRoot;					        //z
				fRoot = 0.5 / fRoot;
				orientation.push( (kQ_02 + kQ_20) * fRoot);		//x
                orientation.push( (kQ_12 + kQ_21) * fRoot);		//y
                orientation.push( z );
                orientation.push( (kQ_10 - kQ_01) * fRoot);		//w
			}
			else{
				//i = 0
				//size_t j = 1;
				//size_t k = 2;
				fRoot = Math.sqrt(kQ_00 - kQ_11 - kQ_22 + 1.0);
				var x = 0.5 * fRoot;					        //x
				fRoot = 0.5 / fRoot;
                orientation.push( x );
				orientation.push( (kQ_10 + kQ_01) * fRoot);	    //y
                orientation.push( (kQ_20 + kQ_02) * fRoot); 	//z
                orientation.push( (kQ_21 - kQ_12) * fRoot);		//w   
			}
		}
	}

    var transform = new Array();
    for(var i=0;i<4;i++)
        transform.push(position[i]);
    for(var i=0;i<4;i++)
        transform.push(orientation[i]);
    for(var i=0;i<4;i++)
        transform.push(scale[i]);
    return transform;
}
/*******************************************************************************
*                                   calcul_Matrix_makeTransform4x4             *
*******************************************************************************/
function calcul_Matrix_makeTransform4x4(transform)	        //posOrientScaleMatrix is 3x4, orient is a quaternion informations, resultTransformMatrix is 4x4              //highly inspired from Ogre Engine
{
    var position = new Array();
    var orientation = new Array();
    var scale = new Array();
    position.push(    transform[0] ); position.push(    transform[1] ); position.push(    transform[2] ); position.push(    transform[3] );
    orientation.push( transform[4] ); orientation.push( transform[5] ); orientation.push( transform[6] ); orientation.push( transform[7] );
    scale.push(       transform[8] ); scale.push(       transform[9] ); scale.push(       transform[10] ); scale.push(      transform[11] );

	// Ordering:
	//    1. Scale
	//    2. Rotate
	//    3. Translate

	//Matrix3 rot3x3;
	//orientation.ToRotationMatrix(rot3x3);
	var fTx = orientation[0] + orientation[0];		// x + x
	var fTy = orientation[1] + orientation[1];		//y + y
	var fTz = orientation[2] + orientation[2];		//z + z
	var fTwx = fTx* orientation[3];		// * w
	var fTwy = fTy* orientation[3];
	var fTwz = fTz* orientation[3];
	var fTxx = fTx* orientation[0];		// * x
	var fTxy = fTy* orientation[0];
	var fTxz = fTz* orientation[0];
	var fTyy = fTy* orientation[1];		// * y
	var fTyz = fTz* orientation[1];
	var fTzz = fTz* orientation[2];		// * z
	var rot3x3_00 = 1.0 - (fTyy + fTzz);
	var rot3x3_01 = fTxy - fTwz;
	var rot3x3_02 = fTxz + fTwy;
	var rot3x3_10 = fTxy + fTwz;
	var rot3x3_11 = 1.0 - (fTxx + fTzz);
	var rot3x3_12 = fTyz - fTwx;
	var rot3x3_20 = fTxz - fTwy;
	var rot3x3_21 = fTyz + fTwx;
	var rot3x3_22 = 1.0 - (fTxx + fTyy);



    // Set up final matrix with scale, rotation and translation
    var matrix = new Array();
	matrix.push(scale[0] * rot3x3_00);	//m00 = scale.x * 
	matrix.push(scale[1] * rot3x3_01);	//m01 = scale.y *
	matrix.push(scale[2] * rot3x3_02);	//m02 = scale.z * 
	matrix.push(position[0]);					//m03 = pos.x
		
	matrix.push(scale[0] * rot3x3_10);
	matrix.push(scale[1] * rot3x3_11);
	matrix.push(scale[2] * rot3x3_12);
	matrix.push(position[1]);					//m13 = pos.y

	matrix.push(scale[0] * rot3x3_20);
	matrix.push(scale[1] * rot3x3_21);
	matrix.push(scale[2] * rot3x3_22);
	matrix.push(position[2]);				//m23 = pos.z

	// No projection term
	matrix.push(0);
	matrix.push(0);
	matrix.push(0);
    matrix.push(position[3]);				//m33 = pos.w
    
    return matrix;
}


/*******************************************************************************
*                                   Quaternion_quatMulQuat                     *
*******************************************************************************/
function Quaternion_quatMulQuat(quatA, quatB)		//from Ogre Quaternion Quaternion::operator* (const Quaternion& rkQ) const
{
	// NOTE:  Multiplication is not generally commutative, so in most cases p*q != q*p.

    var quat = new Array();
    quat.push(quatA[3] * quatB[0] + quatA[0] * quatB[3] + quatA[1] * quatB[2] - quatA[2] * quatB[1]);
    quat.push(quatA[3] * quatB[1] + quatA[1] * quatB[3] + quatA[2] * quatB[0] - quatA[0] * quatB[2]);
    quat.push(quatA[3] * quatB[2] + quatA[2] * quatB[3] + quatA[0] * quatB[1] - quatA[1] * quatB[0]);
    quat.push(quatA[3] * quatB[3] - quatA[0] * quatB[0] - quatA[1] * quatB[1] - quatA[2] * quatB[2]);
    return quat;
}
/*******************************************************************************
*                                   Quaternion_fromAngleAxis                   *
*******************************************************************************/
function Quaternion_fromAngleAxis(angle, axis)			//from Ogre Quaternion::FromAngleAxis
{
	// assert:  axis[] is unit length
	//
	// The quaternion representing the rotation is
	//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

	var fHalfAngle = 0.5 * angle;
    var fSin = Math.sin(fHalfAngle * degTorad);
    
    var quat = new Array();
    quat.push(fSin * axis[0]);
    quat.push(fSin * axis[1]);
    quat.push(fSin * axis[2]);
    quat.push(Math.cos(fHalfAngle * degTorad));     //w
	return quat;
}



/*******************************************************************************
*                                   Quaternion_fromAngleAxis                   *
*******************************************************************************/
function giveAngleOrientationForThisOrientationTaitBryan_XYZ(quat)				//same version, but with this order of rotation, the yaw is on the diqs display by pitch rotation.
{
	var matrix3x3 = quadToRotationMatrix(quat);                        //convert into a matrix3x3
	var angles = matrixToEulerAnglesZYX(matrix3x3);                    //convert matrix3x3 into EulerAngle
	
	return angles;
}



/*******************************************************************************
*                                   Quaternion_fromAngleAxis                   *
*******************************************************************************/
function quadToRotationMatrix(quat)
{
	//normalize quaternion as in https://www.andre-gaschler.com/rotationconverter/ , else we could have infinite + weird result on matrixToEulerAnglesZYX, because of float precision on quaternion.
    var a = Math.sqrt(quat[0] * quat[0] + quat[1] * quat[1] + quat[2] * quat[2] + quat[3] * quat[3]);
    var quat_tmp = new Array();
	if (0 == a)
	{
		quat_tmp[0] = quat_tmp[1] = quat_tmp[2] = 0;
		quat_tmp[3] = 1;
	}else{
		a = 1.0 / a;
		quat_tmp[0] = quat[0] * a;
		quat_tmp[1] = quat[1] * a;
		quat_tmp[2] = quat[2] * a;
		quat_tmp[3] = quat[3] * a;
    }
    
	var fTx = quat_tmp[0] + quat_tmp[0];
	var fTy = quat_tmp[1] + quat_tmp[1];
	var fTz = quat_tmp[2] + quat_tmp[2];
	var fTwx = fTx * quat_tmp[3];
	var fTwy = fTy * quat_tmp[3];
	var fTwz = fTz * quat_tmp[3];
	var fTxx = fTx * quat_tmp[0];
	var fTxy = fTy * quat_tmp[0];
	var fTxz = fTz * quat_tmp[0];
	var fTyy = fTy * quat_tmp[1];
	var fTyz = fTz * quat_tmp[1];
	var fTzz = fTz * quat_tmp[2];

    var matrix = new Array();
	matrix.push(1.0 - (fTyy + fTzz));
	matrix.push(fTxy - fTwz);
	matrix.push(fTxz + fTwy);
	matrix.push(fTxy + fTwz);
	matrix.push(1.0 - (fTxx + fTzz));
	matrix.push(fTyz - fTwx);
	matrix.push(fTxz - fTwy);
	matrix.push(fTyz + fTwx);
    matrix.push(1.0 - (fTxx + fTyy));
    return matrix;
}


/*******************************************************************************
*                                   Quaternion_fromAngleAxis                   *
*******************************************************************************/
function matrixToEulerAnglesZYX(matrix3x3)
{
	// rot =  cy*cz           cz*sx*sy-cx*sz  cx*cz*sy+sx*sz
	//        cy*sz           cx*cz+sx*sy*sz -cz*sx+cx*sy*sz
	//       -sy              cy*sx           cx*cy

    var matrix = new Array();
    for (var i = 0; i < 9; i++)
        matrix.push(matrix3x3[i]);

	for (var i = 0; i < 3; i++)		//few corrections, due to the float precision on quaternion.
	{
		if ((matrix[i] < -1) && (Math.abs(matrix[i] - (-1)) < 0.000001))
            matrix[i] = -1;
		if ((matrix[i] > 1) && (Math.abs(matrix[i] - 1) < 0.000001))
            matrix[i] = 1;

		if ((matrix[3 + i] < -1) && (Math.abs(matrix[3 + i] - (-1)) < 0.000001))
            matrix[3 + i] = -1;
		if ((matrix[3 + i] > 1) && (Math.abs(matrix[3 + i] - 1) < 0.000001))
            matrix[3 + i] = 1;
		
		if ((matrix[6 + i] < -1) && (Math.abs(matrix[6 + i] - (-1)) < 0.000001))
            matrix[6 + i] = -1;
		if ((matrix[6 + i] > 1) && (Math.abs(matrix[6 + i] - 1) < 0.000001))
            matrix[6 + i] = 1;
	}


    var angles = [0,0,0];
	angles[1] = Math.sin(-matrix[6 + 0] ) * radTodeg;
	if (angles[1] < 90.0)
	{
		if (angles[1] > -90.0)
		{
			angles[0] = Math.atan(matrix[3 + 0], matrix[0]) * radTodeg;
			angles[2] = Math.atan(matrix[6 + 1], matrix[6 + 2]) * radTodeg;
			//return true;
		}else{
			// WARNING.  Not a unique solution.
			var fRmY = Math.atan(-matrix[1], matrix[2]) * radTodeg;
			angles[2] = 0.0;  // any angle works
			//angles[0] = angles[2] - fRmY;
			angles[0] = fRmY - YPR_angles[2];
			//return false;
		}
	}else{
		// WARNING.  Not a unique solution.
		var fRpY = Math.atan(-matrix[1], matrix[2]);
		angles[2] = 0.0;  // any angle works
		angles[0] = fRpY - angles[2];
		//return false;
    }
    
    return angles;
}


















////////////////////////////////////////////////// Json
JSON.toString = JSON.stringify;
JSON.clone = function( json ) { return JSON.parse(JSON.stringify(json)); }

JSON.toString_pretty = function syntaxHighlight(json) { return JSON.stringify(json, null, 2); }
JSON.toString_prettyColored = function(json)                          // https://stackoverflow.com/questions/4810841/how-can-i-pretty-print-json-using-javascript
{
    if (typeof json != 'string')
         json = JSON.stringify(json, undefined, 2);

    json = json.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
    return '<pre class="json">'+ json.replace(/("(\\u[a-zA-Z0-9]{4}|\\[^u]|[^\\"])*"(\s*:)?|\b(true|false|null)\b|-?\d+(?:\.\d*)?(?:[eE][+\-]?\d+)?)/g, function (match)
    {
        var cls = 'json_number';
        if (/^"/.test(match))
        {
            if (/:$/.test(match))
                cls = 'json_key';
            else 
                cls = 'json_string';
        } else if (/true|false/.test(match)) {
            cls = 'json_boolean';
        } else if (/null/.test(match)) {
            cls = 'json_null';
        }
        return '<span class="' + cls + '">' + match + '</span>';
    }) + "</pre>";
}


////////////////////////////////////////////////// Html


function htmlToText(html)
{
   var tmp = document.createElement("DIV");
   tmp.innerHTML = html;
   return tmp.textContent || tmp.innerText || "";
}

jQuery.fn.outerHTML = function() { return jQuery('<div />').append(this.eq(0).clone()).html(); };

function cssPropStrPxToNumber( strValue )               // to convert "132px" => 132
{
    return Number( strValue.substr(0, strValue.length-2) );
}


function guidGenerator()
{
    var S4 = function() 
    {
       return (((1+Math.random())*0x10000)|0).toString(16).substring(1);
    };
    return (S4()+S4()+"-"+S4()+"-"+S4()+"-"+S4()+"-"+S4()+S4()+S4());
}


function scrollToPos_Textarea(jq_textArea, position)            //for classic with oveflowy= auto
{
    if((!ckv(jq_textArea))||(jq_textArea.length==0))
        return;

    textArea = jq_textArea[0];

    if(!ckv(position))
        position = (textArea.selectionStart + textArea.selectionEnd) / 2;
    if(position>=jq_textArea.val().length)
        position = (jq_textArea.val().length!=0) ? (jq_textArea.val().length-1) : 0;


    var charsPerRow = textArea.cols;
    if(charsPerRow==0)
        charsPerRow = 1;                                                //to avoid division by zero
    var nbRows = (textArea.rows!=0) ? textArea.rows : 1;                //to avoid division by zero


    var selectionRow = (position - (position % charsPerRow)) / charsPerRow;
    var lineHeight = textArea.clientHeight / nbRows;
    textArea.scrollTop = lineHeight * selectionRow;
}


function scrollToPos_Textarea_overflowX(jq_textArea, position)            //for classic with oveflowy= auto
{
    if((!ckv(jq_textArea))||(jq_textArea.length==0))
        return;

    textArea = jq_textArea[0];
    var text = jq_textArea.val();
    
    if(!ckv(position))
        position = (textArea.selectionStart>0) ? (textArea.selectionStart-1) : 0;
    if(position>=text.length)
        position = (text.length!=0) ? (text.length-1) : 0;

    var lines = text.split("\n");
    
    var total_previousLines = 0;
    var indexLine = 0;
    var maxChars = 0;
    var isdetected = false;
    var nbLines = lines.length;
    for(var i=0;i<nbLines;i++)
    {
        var nbChars = lines[i].length + 1;                  //+1 for \n
        if(nbChars>maxChars)
            maxChars = nbChars;

        if(isdetected)                      //to continue maxChars after detect the line of position
            continue;

        if(total_previousLines + nbChars > position)
        {
            isdetected = true;
            continue;
        }
        total_previousLines += nbChars;
        indexLine = i;
    }
    var indexInLine = (position - total_previousLines);

    var oldNbLine = nbLines;
    var oldMaxChars = maxChars;

    if(nbLines<2)
        nbLines = 2;                                //to avoid division by zero
    if(maxChars<2)
        maxChars = 2;

    var scrollLeft = textArea.scrollWidth * (indexInLine / (maxChars-1));
    var scrollTop = textArea.scrollHeight * (indexLine   / (nbLines-1) );

    scrollLeft -= textArea.clientWidth/2;                          //try to put the cursor on the middle of the textArea.
    if(scrollLeft < 0)
        scrollLeft = 0;
       
    scrollTop -= textArea.clientHeight/2;                          //try to put the cursor on the middle of the textArea.
    if(scrollTop < 0)
        scrollTop = 0;
    
    textArea.scrollLeft = scrollLeft;
    textArea.scrollTop = scrollTop;
    

    //console.log("pos:"+ position +" => c"+ indexInLine +"/"+ maxChars +" L"+ indexLine +"/"+ oldNbLine +" => s("+ textArea.scrollLeft +","+ textArea.scrollTop +")"   );
}



var requestFullScreen = window.document.documentElement.requestFullscreen || window.document.documentElement.mozRequestFullScreen || window.document.documentElement.webkitRequestFullscreen || window.document.documentElement.msRequestFullscreen;
var cancelFullScreen = window.document.exitFullscreen || window.document.mozCancelFullScreen || window.document.webkitExitFullscreen || window.document.msExitFullscreen;
function isFullScreenAvailable()  { return ckv(requestFullScreen); }
function setFullScreen(enable)
{
    if(!ckv(requestFullScreen))
        return;
    
    if(enable)
        requestFullScreen.call(window.document.documentElement);
    else
        cancelFullScreen.call(window.document);
}
function isFullScreen() { return (!(!window.document.fullscreenElement && !window.document.mozFullScreenElement && !window.document.webkitFullscreenElement && !window.document.msFullscreenElement)); }
function toggleFullScreen() { setFullScreen(!isFullScreen()); }



////////////////////////////////////////////////// Canva


function isCanvasSupported()                    //https://stackoverflow.com/questions/2745432/best-way-to-detect-that-html5-canvas-is-not-supported
{
    var elem = document.createElement('canvas');
    return !!(elem.getContext && elem.getContext('2d'));
}



////////////////////////////////////////////////// Local Storage

function isStorageActive() { return (typeof(Storage) !== "undefined"); }





////////////////////////////////////////////////// Detection


function is_touch_device()
{
    var prefixes = ' -webkit- -moz- -o- -ms- '.split(' ');
    var mq = function(query)
    {
        return window.matchMedia(query).matches;
    }
  
    if (('ontouchstart' in window) || window.DocumentTouch && document instanceof DocumentTouch)
        return true;
  
    // include the 'heartz' as a way to have a non matching MQ to help terminate the join https://git.io/vznFH
    var query = ['(', prefixes.join('touch-enabled),('), 'heartz', ')'].join('');
    return mq(query);
}
//alert("userAgent : "+ navigator.userAgent + "\nvendor : "+ navigator.vendor);
function is_fireFox(){ return (navigator.userAgent.toLowerCase().indexOf('firefox') > -1); }
function is_chrome() { return ((/Chrome/.test(navigator.userAgent) && /Google Inc/.test(navigator.vendor)) || (/CriOS/.test(navigator.userAgent))); }
function is_safari() { return /Safari/.test(navigator.userAgent) && /Apple Computer/.test(navigator.vendor); }
function is_ios() { return /iPad|iPhone|iPod/.test(navigator.userAgent) && !window.MSStream; }      // https://stackoverflow.com/questions/9038625/detect-if-device-is-ios


function ios_version()
{
    if((!is_ios())||(!(/iP(hone|od|ad)/.test(navigator.platform))))
        return null;
    
    var v = (navigator.appVersion).match(/OS (\d+)_(\d+)_?(\d+)?/);               // supports iOS 2.0 and later: <http://bit.ly/TJjs1V>
    return [parseInt(v[1], 10), parseInt(v[2], 10), parseInt(v[3] || 0, 10)];
}


function is_windows(){ return (navigator.userAgent.toLowerCase().indexOf('windows') > -1); }
function windows_version()
{ 
    var pos = navigator.userAgent.toLowerCase().indexOf('windows ');
    if(pos==-1)
        return null;

    var str = navigator.userAgent.toLowerCase().substr(pos + 8);
    if(str.indexOf(";")!=-1)
        str = str.substr(0, str.indexOf(";"));

    var sv = str.split(" ");
    if(sv.length!=2)
        return null;
    
    var sv_b = sv[1].split(".");
    var version = {type: sv[0], major: Number(sv_b[0]), minor: ((sv_b.length>1) ? Number(sv_b[1]) : null), patch: ((sv_b.length>2) ? Number(sv_b[2]) : null), typeSystem: ((navigator.userAgent.toLowerCase().indexOf("win64")!=-1) ? 64 : 32)};
    
    return version;
}




