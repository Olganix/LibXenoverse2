/**
 * Copyright (c) Olganix; Licensed
 */

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// Matrix ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

//todo: on hover see full number.
//may be add button to small size;

var MATRIX_UNIQUE_ID = 0;

class Matrix
{
    constructor(divParent, handlerParent, boneInstance)
    {
        if((divParent==undefined)||(divParent==null))
            divParent = "body";
    
        this.id = MATRIX_UNIQUE_ID;
        MATRIX_UNIQUE_ID++;

        this.name = (((boneInstance)&&(boneInstance.name)) ? ( ((boneInstance.index) ? (boneInstance.index + ": ") : "") +boneInstance.name) : ('Matrix'+ this.id) );
        this.divParent = divParent;
        this.handlerParent = handlerParent;
        this.boneInstance = boneInstance;
        this.type = null;                           //Matrix4x4, Matrix3x3, Matrix2x2, Matrix4x3, PRS_3d (position, rotation, scale), POS_3d (Position, Orientation(= Quaternion), Scale).... for now we only deal with Matrix4x4, PRS_3d ou POS_3D
        this.values = null;

        this.iddentityMatrix4x4_values = [1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1];
        this.iddentityTransformPRS_values = [0,0,0,1, 0,0,0,   1,1,1,1];
        this.iddentityTransformPOS_values = [0,0,0,1, 0,0,0,1, 1,1,1,1];

        this.errors_str = "";
        
        
        $(divParent).append('\
<div id="Matrix_'+ this.id +'" class="Matrix_Panel">\
    <div class="Matrix_header">\
        <div class="Matrix_header_top">\
            <div class="Matrix_header_top_name">'+ this.name +'</div>\
            <div class="Matrix_header_top_hide"></div>\
            <div class="Matrix_header_top_delete">X</div>\
        </div>\
        <div class="Matrix_header_middle">\
            <div class="Matrix_header_middle_left">\
                <div class="bt_abs bt">A</div>\
                <div class="bt_rel bt active">R</div>\
                <div class="bt_fillXml">Fill</div>\
            </div>\
            <div class="Matrix_header_middle_right">\
                <div class="bt_Matrix bt active">M</div>\
                <div class="bt_Transform bt active">T</div>\
                <div class="bt_RotVsOrient bt active">Q</div>\
            </div>\
        </div>\
    </div>\
    <div class="Matrix_body">\
        <div class="Matrix_body_abs">\
            <div class="Matrix_view hide">\
                <div class="Matrix_line">\
                    <input class="Matrix_cell Matrix_00" type="text" value="1" />\
                    <input class="Matrix_cell Matrix_01" type="text" value="0" />\
                    <input class="Matrix_cell Matrix_02" type="text" value="0" />\
                    <input class="Matrix_cell Matrix_03" type="text" value="0" />\
                </div>\
                <div class="Matrix_line">\
                    <input class="Matrix_cell Matrix_10" type="text" value="0" />\
                    <input class="Matrix_cell Matrix_11" type="text" value="1" />\
                    <input class="Matrix_cell Matrix_12" type="text" value="0" />\
                    <input class="Matrix_cell Matrix_13" type="text" value="0" />\
                </div>\
                <div class="Matrix_line">\
                    <input class="Matrix_cell Matrix_20" type="text" value="0" />\
                    <input class="Matrix_cell Matrix_21" type="text" value="0" />\
                    <input class="Matrix_cell Matrix_22" type="text" value="1" />\
                    <input class="Matrix_cell Matrix_23" type="text" value="0" />\
                </div>\
                <div class="Matrix_line">\
                    <input class="Matrix_cell Matrix_30" type="text" value="0" />\
                    <input class="Matrix_cell Matrix_31" type="text" value="0" />\
                    <input class="Matrix_cell Matrix_32" type="text" value="0" />\
                    <input class="Matrix_cell Matrix_33" type="text" value="1" />\
                </div>\
            </div>\
            <div class="Transform_view hide">\
                <div class="Transform_line position">\
                    <input class="Transform_cell Transform_Pos_x" type="text" value="0" />\
                    <input class="Transform_cell Transform_Pos_y" type="text" value="0" />\
                    <input class="Transform_cell Transform_Pos_z" type="text" value="0" />\
                    <input class="Transform_cell Transform_Pos_w" type="text" value="1" />\
                </div>\
                <div class="Transform_line rotation">\
                    <input class="Transform_cell Transform_Rot_x" type="text" value="0" />\
                    <input class="Transform_cell Transform_Rot_y" type="text" value="0" />\
                    <input class="Transform_cell Transform_Rot_z" type="text" value="0" />\
                </div>\
                <div class="Transform_line orientation hide">\
                    <input class="Transform_cell Transform_Quat_x" type="text" value="0" />\
                    <input class="Transform_cell Transform_Quat_y" type="text" value="0" />\
                    <input class="Transform_cell Transform_Quat_z" type="text" value="0" />\
                    <input class="Transform_cell Transform_Quat_w" type="text" value="1" />\
                </div>\
                <div class="Transform_line scale">\
                    <input class="Transform_cell Transform_Scl_x" type="text" value="1" />\
                    <input class="Transform_cell Transform_Scl_y" type="text" value="1" />\
                    <input class="Transform_cell Transform_Scl_z" type="text" value="1" />\
                    <input class="Transform_cell Transform_Scl_w" type="text" value="1" />\
                </div>\
            </div>\
        </div>\
        <div class="Matrix_body_rel">\
            <div class="Matrix_view hide">\
                <div class="Matrix_line">\
                    <input class="Matrix_cell Matrix_00" type="text" value="1" />\
                    <input class="Matrix_cell Matrix_01" type="text" value="0" />\
                    <input class="Matrix_cell Matrix_02" type="text" value="0" />\
                    <input class="Matrix_cell Matrix_03" type="text" value="0" />\
                </div>\
                <div class="Matrix_line">\
                    <input class="Matrix_cell Matrix_10" type="text" value="0" />\
                    <input class="Matrix_cell Matrix_11" type="text" value="1" />\
                    <input class="Matrix_cell Matrix_12" type="text" value="0" />\
                    <input class="Matrix_cell Matrix_13" type="text" value="0" />\
                </div>\
                <div class="Matrix_line">\
                    <input class="Matrix_cell Matrix_20" type="text" value="0" />\
                    <input class="Matrix_cell Matrix_21" type="text" value="0" />\
                    <input class="Matrix_cell Matrix_22" type="text" value="1" />\
                    <input class="Matrix_cell Matrix_23" type="text" value="0" />\
                </div>\
                <div class="Matrix_line">\
                    <input class="Matrix_cell Matrix_30" type="text" value="0" />\
                    <input class="Matrix_cell Matrix_31" type="text" value="0" />\
                    <input class="Matrix_cell Matrix_32" type="text" value="0" />\
                    <input class="Matrix_cell Matrix_33" type="text" value="1" />\
                </div>\
            </div>\
            <div class="Transform_view hide">\
                <div class="Transform_line position">\
                    <input class="Transform_cell Transform_Pos_x" type="text" value="0" />\
                    <input class="Transform_cell Transform_Pos_y" type="text" value="0" />\
                    <input class="Transform_cell Transform_Pos_z" type="text" value="0" />\
                    <input class="Transform_cell Transform_Pos_w" type="text" value="1" />\
                </div>\
                <div class="Transform_line rotation">\
                    <input class="Transform_cell Transform_Rot_x" type="text" value="0" />\
                    <input class="Transform_cell Transform_Rot_y" type="text" value="0" />\
                    <input class="Transform_cell Transform_Rot_z" type="text" value="0" />\
                </div>\
                <div class="Transform_line orientation hide">\
                    <input class="Transform_cell Transform_Quat_x" type="text" value="0" />\
                    <input class="Transform_cell Transform_Quat_y" type="text" value="0" />\
                    <input class="Transform_cell Transform_Quat_z" type="text" value="0" />\
                    <input class="Transform_cell Transform_Quat_w" type="text" value="1" />\
                </div>\
                <div class="Transform_line scale">\
                    <input class="Transform_cell Transform_Scl_x" type="text" value="1" />\
                    <input class="Transform_cell Transform_Scl_y" type="text" value="1" />\
                    <input class="Transform_cell Transform_Scl_z" type="text" value="1" />\
                    <input class="Transform_cell Transform_Scl_w" type="text" value="1" />\
                </div>\
            </div>\
        </div>\
    </div>\
</div>\
'); 
        var main_jq = $('#Matrix_'+ this.id);

        

        
        var context = this;                         //for actions
        

        main_jq.find(".Matrix_header_top_hide").click(function()
        {
            var list = $(this).parent().parent().parent().find(".Matrix_body, .Matrix_header_middle");
            
            $(this).toggleClass("active");
            
            if($(this).hasClass("active"))
                list.addClass("hide");
            else
                list.removeClass("hide");
        });

        main_jq.find(".Matrix_header_top_delete").click(function()
        {
            if(ckv(context.boneInstance))
                context.boneInstance.notifyMatrixRemoved(context);
            
            if(ckv(context.handlerParent))
                context.handlerParent.notifyMatrixRemoved(context);
        });

        

        main_jq.find(".bt_abs").click(function()
        {
            $(this).toggleClass("active");
            $(this).parent().parent().parent().parent().find(".Matrix_body_abs, .Matrix_body_rel").toggleClass("abs");
        });
        if(main_jq.find(".bt_abs").hasClass("active"))
            main_jq.find(".Matrix_body_abs, .Matrix_body_rel").addClass("abs");
        



        main_jq.find(".bt_rel").click(function()
        {
            $(this).toggleClass("active");
            $(this).parent().parent().parent().parent().find(".Matrix_body_abs, .Matrix_body_rel").toggleClass("rel");
        });
        if(main_jq.find(".bt_rel").hasClass("active"))
            main_jq.find(".Matrix_body_abs, .Matrix_body_rel").addClass("rel");




        main_jq.find(".bt_Matrix").click(function()
        {
            $(this).toggleClass("active");
            $(this).parent().parent().parent().parent().find(".Matrix_view").toggleClass("hide");
        });
        if(main_jq.find(".bt_Matrix").hasClass("active"))
            main_jq.find(".Matrix_view").removeClass("hide");
        



        main_jq.find(".bt_Transform").click(function()
        {
            $(this).toggleClass("active");
            $(this).parent().parent().parent().parent().find(".Transform_view").toggleClass("hide");
        });
        if(main_jq.find(".bt_Transform").hasClass("active"))
            main_jq.find(".Transform_view").removeClass("hide");



        main_jq.find(".bt_RotVsOrient").click(function()
        {
            $(this).toggleClass("active");
            $(this).parent().parent().parent().parent().find(".Transform_line.rotation, .Transform_line.orientation").toggleClass("hide");
        });
        if(main_jq.find(".bt_RotVsOrient").hasClass("active"))
            main_jq.find(".Transform_line.rotation, .Transform_line.orientation").toggleClass("hide");






        main_jq.find("input").change(function()
        {
            var value = $(this).val();
            if(!isNumber_b(value))
            {
                alert("this field must ba a number. so reverting");
                $(this).val($(this).attr("prevValue"));
                return;
            }
            var isAbsolute = ($(this).parents(".Matrix_body_abs").length!=0);
            var isMatrix = ($(this).parents(".Matrix_view").length!=0);

            var isAllOk = true;

            if(isMatrix)
            {
                isAllOk = isAllOk && context.updateMatrix(isAbsolute);
                isAllOk = isAllOk && context.updateTransformFromMatrix(isAbsolute);
                isAllOk = isAllOk && context.updateTransformFromMatrix(!isAbsolute);
            }else{
                
                var isOrient = ($(this).parents(".orientation").length!=0);
                if(isOrient)
                    isAllOk = isAllOk && context.updateRotFromQuat(isAbsolute);
                else
                    isAllOk = isAllOk && context.updateQuatFromRot(isAbsolute);

                isAllOk = isAllOk && context.updateMatrixFromTransform(isAbsolute);    
                isAllOk = isAllOk && context.updateMatrix(isAbsolute);
                isAllOk = isAllOk && context.updateTransformFromMatrix(!isAbsolute);
            }

            if(isAllOk)
            {
                context.updatePrevValues();

                context.updateBoneInstance();
            }else{
                alert("There is some error on Matrix "+ context.name +" between conversion operations, so reverting: \n"+ context.errors_str);
                context.errors_str = "";

                context.revertValues();
            }
        });




        main_jq.find(".bt_fillXml").click(function()
        {
            $(this).toggleClass("active");

            var jq_div = $(this).parent().parent().parent().parent();
            var id = jq_div[0].id;

            if($(this).hasClass("active"))
            {
                
                jq_div.append('\
<div class="fill_panel">\
    <textarea class="ta_fill" name="fast fill matrix with Xml" placeholder="put some Xml here. could be fpf Transform Xml (from Lazybone Tools), Esk\'s AbsoluteTransformMatrix or Esk\'s RelativeTransform" value="" ></textarea>\
    <div class="bt_Esk_Abs_Xml bt">Get EskAbs</div>\
    <div class="bt_Esk_Rel_Xml bt">Get EskRel</div>\
    <div class="bt_Fpf_Xml bt">Get Fpf</div>\
    <div class="bt_fill_matrix bt">Fill Matrix</div>\
    <div class="bt_close">X</div>\
</div>\
');
                jq_div.find(".ta_fill").keydown(function(e)
                {
                    if (e.keyCode == 65 && e.ctrlKey) 
                    {
                        e.target.select();
                    }
                });

                jq_div.find(".bt_fill_matrix").click(function()
                {
                    var xml_str = jq_div.find(".ta_fill").val();
                    if((xml_str.indexOf("F_00")!=-1)&&(xml_str.indexOf("F_04")!=-1)&&(xml_str.indexOf("F_08")!=-1)&&(xml_str.indexOf("F_12")!=-1)&&(xml_str.indexOf("F_16")!=-1)&&(xml_str.indexOf("F_20")!=-1)&&(xml_str.indexOf("F_24")!=-1)&&(xml_str.indexOf("F_28")!=-1)&&(xml_str.indexOf("F_32")!=-1)&&(xml_str.indexOf("F_36")!=-1)&&(xml_str.indexOf("F_40")!=-1)&&(xml_str.indexOf("F_44")!=-1)&&(xml_str.indexOf("F_48")!=-1)&&(xml_str.indexOf("F_52")!=-1)&&(xml_str.indexOf("F_56")!=-1)&&(xml_str.indexOf("F_60")!=-1))
                    {                                               //it's Transform from Lazybone Fpf Xml.
                        var jq_doc = $(xml_str);
                        var values = new Array();
                        for(var i=0;i<=60;i+=4)
                            values.push(Number( jq_doc.find("F_"+ numberTostring(i, 2) ).attr("value") ));

                        context.setValues(values, "Matrix4x4", false);
                        context.updateMatrix(false);
                        context.updateTransformFromMatrix(true);
                        context.updateTransformFromMatrix(false);




                    }else if((xml_str.indexOf("Position")!=-1)&&(xml_str.indexOf("Orientation")!=-1)&&(xml_str.indexOf("Scale")!=-1)){
                                                                    //it's ESK RelativeTransform
                        
                        var jq_doc = $(xml_str);
                        var values = new Array();
                        for(var i=0;i<3;i++)
                        {
                            var prefix = "";
                            switch(i)
                            {
                                case 0: prefix = "Position"; break;
                                case 1: prefix = "Orientation"; break;
                                case 2: prefix = "Scale"; break;
                            }

                            for(var j=0;j<4;j++)
                            {
                                var sufix = "";
                                switch(j)
                                {
                                    case 0: sufix = "x"; break;
                                    case 1: sufix = "y"; break;
                                    case 2: sufix = "z"; break;
                                    case 3: sufix = "w"; break;
                                }
                                values.push(jq_doc.find(prefix).attr(sufix));
                            }
                        }
                        context.setValues(values, "POS_3d", false);
                        context.updateRotFromQuat(false);
                        context.updateMatrixFromTransform(false);    
                        context.updateMatrix(false);
                        context.updateTransformFromMatrix(true);




                    }else if((xml_str.indexOf("Line")!=-1)){
                                                                    //it's ESK AbsoluteTransformMatrix

                        var jq_doc = $(xml_str);
                        var lines = jq_doc.find("Line");

                        var values = new Array();
                        for(var i=0;i<lines.length;i++)
                        {
                            for(var j=0;j<4;j++)
                            {
                                var sufix = "";
                                switch(j)
                                {
                                    case 0: sufix = "x"; break;
                                    case 1: sufix = "y"; break;
                                    case 2: sufix = "z"; break;
                                    case 3: sufix = "w"; break;
                                }
                                values.push( $(lines[i]).attr(sufix) );
                            }
                        }
                        context.setValues(values, "Matrix4x4", true);
                        context.updateMatrix(true);
                        context.updateTransformFromMatrix(true);
                        context.updateTransformFromMatrix(false);
                        

                    }else{
                        alert("Error: xml is not understable. try Esk RelativeTransform, Esk AbsoluteTransformMatrix, or fpf Transform.")
                    }
                });



                jq_div.find(".bt_Esk_Abs_Xml").click(function()
                {
                    var values = context.getValues("Matrix4x4", true);

                    var str = "<AbsoluteTransformMatrix>\n";
                    for(var i=0;i<4;i++)
                    {
                        str += '\t<Line ';
                        for(var j=0;j<4;j++)
                        {
                            var sufix = "";
                            switch(j)
                            {
                                case 0: sufix = "x"; break;
                                case 1: sufix = "y"; break;
                                case 2: sufix = "z"; break;
                                case 3: sufix = "w"; break;
                            }
                            str += sufix +'="'+ values[i*4 + j] +'" ';
                        }
                        str += '/>\n';
                    }
                    str += "</AbsoluteTransformMatrix>\n";

                    jq_div.find(".ta_fill").val(str);
                });




                jq_div.find(".bt_Esk_Rel_Xml").click(function()
                {
                    var values = context.getValues("POS_3d", false);

                    var str = "<RelativeTransform>\n";
                    for(var i=0;i<3;i++)
                    {
                        var prefix = "";
                        switch(i)
                        {
                            case 0: prefix = "Position"; break;
                            case 1: prefix = "Orientation"; break;
                            case 2: prefix = "Scale"; break;
                        }


                        str += '\t<'+ prefix +' ';
                        for(var j=0;j<4;j++)
                        {
                            var sufix = "";
                            switch(j)
                            {
                                case 0: sufix = "x"; break;
                                case 1: sufix = "y"; break;
                                case 2: sufix = "z"; break;
                                case 3: sufix = "w"; break;
                            }
                            str += sufix +'="'+ values[i*4 + j] +'" ';
                        }
                        str += '/>\n';
                    }
                    str += "</RelativeTransform>\n";

                    jq_div.find(".ta_fill").val(str);
                });





                jq_div.find(".bt_Fpf_Xml").click(function()
                {
                    var values = context.getValues("Matrix4x4", false);

                    var str = "<TransformX>\n";
                    for(var i=0;i<16;i++)
                        str += '\t<F_'+ numberTostring(i * 4, 2) +' value="'+ values[i] +'" />\n';
                    str += "</TransformX>\n";

                    jq_div.find(".ta_fill").val(str);
                });

                jq_div.find(".bt_close").click(function()
                {
                    jq_div.find(".bt_fillXml").removeClass("active");
                    jq_div.find(".fill_panel").remove();
                });

            }else{
                jq_div.find(".fill_panel").remove();
            }
        });
        

        
        this.updatePrevValues();

        if(ckv(this.boneInstance))
            this.setValues_withUpdates(this.boneInstance.getAbsoluteMatrix(), true)
    }

    destroy()
    {
        $('#Matrix_'+ this.id).remove();
    }








    



    /*******************************************************************************
    *                                   setValues_withUpdates                      *
    *******************************************************************************/
    setValues_withUpdates(valuesStruct, isAbsolute)
    {
        var isAllOk = true;
        
        this.setValues(valuesStruct.values, valuesStruct.type, isAbsolute);

        if(valuesStruct.type=="Matrix4x4")
        {
            isAllOk = isAllOk && this.updateMatrix(!isAbsolute);
            isAllOk = isAllOk && this.updateTransformFromMatrix(true);
            isAllOk = isAllOk && this.updateTransformFromMatrix(false);

        }else if(valuesStruct.type=="PRS_3d"){              //Pos Rot Scale

            isAllOk = isAllOk && this.updateQuatFromRot(isAbsolute);
            isAllOk = isAllOk && this.updateMatrixFromTransform(isAbsolute);
            isAllOk = isAllOk && this.updateMatrix(!isAbsolute);
            isAllOk = isAllOk && this.updateTransformFromMatrix(!isAbsolute);

        }else if(valuesStruct.type=="POS_3d"){              //Pos Orient (Quaternion) Scale 
            
            isAllOk = isAllOk && this.updateRotFromQuat(isAbsolute);
            isAllOk = isAllOk && this.updateMatrixFromTransform(isAbsolute);
            isAllOk = isAllOk && this.updateMatrix(!isAbsolute);
            isAllOk = isAllOk && this.updateTransformFromMatrix(!isAbsolute);
        }

        if(isAllOk)
        {
            this.updatePrevValues();
        }else{
            alert("There is some error on Matrix "+ this.name +" between conversion operations, so reverting: \n"+ this.errors_str);
            this.errors_str = "";

            this.revertValues();
        }
    }

    /*******************************************************************************
    *                                   updateBoneInstance                         *
    *******************************************************************************/
    updateBoneInstance()
    {
        if(!ckv(this.boneInstance))
            return;

        var values = this.getValues("Matrix4x4", true);
        this.boneInstance.setAbsoluteMatrix({type: "Matrix4x4", values});           //todo inside , update all child, witch also will update all visual absolute Matrix
    }





    /*******************************************************************************
    *                                   updateMatrix                               *
    *******************************************************************************/
    updateMatrix(isAbsolute)                        //calcul relative to absolute (and vis-versa) job.
    {
        var values = this.getValues("Matrix4x4", isAbsolute);

        if((!ckv(this.boneInstance))||(!this.boneInstance.getParentAbsoluteMatrix()))
        {
            this.setValues(values, "Matrix4x4", !isAbsolute);

        }else{

            var newValues = new Array();
            var parentMatrixValues = this.boneInstance.getParentAbsoluteMatrix().values;

            if(isAbsolute)
                newValues = calcul_Dbvx_RelativeMatrix4x4(values, parentMatrixValues);
            else
                newValues = calcul_Dbvx_AbsoluteMatrix4x4(values, parentMatrixValues);

            if(ckv(newValues.errors_str))
            {
                this.errors_str = newValues.errors_str;
                return false;
            }

            this.setValues(newValues, "Matrix4x4", !isAbsolute);
        }

        return true;
    }


    /*******************************************************************************
    *                                   updateTransformFromMatrix                  *
    *******************************************************************************/
    updateTransformFromMatrix(isAbsolute)
    {
        var values = this.getValues("Matrix4x4", isAbsolute);
        
        var newValues = calcul_Dbvx_TransformFromMatrix4x4(values);
        if(ckv(newValues.errors_str))
        {
            this.errors_str = newValues.errors_str;
            return false;
        }

        this.setValues(newValues, "POS_3d", isAbsolute);

        this.updateRotFromQuat(isAbsolute);

        return true;
    }
    /*******************************************************************************
    *                                   updateMatrixFromTransform                  *
    *******************************************************************************/
    updateMatrixFromTransform(isAbsolute)
    {
        var values = this.getValues("POS_3d", isAbsolute);

        var newValues = calcul_Dbvx_Matrix4x4FromTransform(values);
        if(ckv(newValues.errors_str))
        {
            this.errors_str = newValues.errors_str;
            return false;
        }

        this.setValues(newValues, "Matrix4x4", isAbsolute);

        return true;
    }



    /*******************************************************************************
    *                                   updateQuatFromRot                          *
    *******************************************************************************/
    updateQuatFromRot(isAbsolute)
    {
        var values = this.getRotValues(isAbsolute);

        var quatX = Quaternion_fromAngleAxis(values[0], [1, 0, 0]);    //rotX
        var quatY = Quaternion_fromAngleAxis(values[1], [0, 1, 0]);    //rotY
        var quatZ = Quaternion_fromAngleAxis(values[2], [0, 0, 1]);    //rotZ

        var newValues = Quaternion_quatMulQuat(quatZ, Quaternion_quatMulQuat(quatY, quatX)); //XYZ		=> not like the XYZ order in ean, strange. (but it's the only order solution, others give weird)

        this.setQuatValues(newValues, isAbsolute);

        return true;
    }

    /*******************************************************************************
    *                                   updateRotFromQuat                          *
    *******************************************************************************/
    updateRotFromQuat(isAbsolute)
    {
        var values = this.getQuatValues(isAbsolute);

        var angles = giveAngleOrientationForThisOrientationTaitBryan_XYZ(values);
        if(ckv(angles.errors_str))
        {
            this.errors_str = angles.errors_str;
            return false;
        }

        var newValues = new Array();                //Xenoverse data is on XYZ order.
        newValues.push(angles[2]);					//rotation for X axis == roll
        newValues.push(angles[1]);					//rotation for Y axis == yaw on disc from pitch
        newValues.push(angles[0]);					//rotation for Z axis == pitch

        this.setRotValues(newValues, isAbsolute);

        return true;
    }




    /*******************************************************************************
    *                                   updatePrevValues                           *
    *******************************************************************************/
    updatePrevValues()              // add add previous value to be able to revert.
    {
        var list = $('#Matrix_'+ this.id).find(".Matrix_cell, .Transform_cell");
        for(var i=0,nb=list.length;i<nb;i++)
            $(list[i]).attr("prevValue", $(list[i]).val()); 
    }
    /*******************************************************************************
    *                                   revertValues                               *
    *******************************************************************************/
    revertValues()
    {
        var list = $('#Matrix_'+ this.id).find(".Matrix_cell, .Transform_cell");
        for(var i=0,nb=list.length;i<nb;i++)
            $(list[i]).val( $(list[i]).attr("prevValue") ); 
    }


    

    /*******************************************************************************
    *                                   setValues                                  *
    *******************************************************************************/
    setValues(values, type, isAbsolute)
    {
        var spaceSide_jq = $('#Matrix_'+ this.id).find( (isAbsolute) ? ".Matrix_body_abs" : ".Matrix_body_rel");
        if(type=="Matrix4x4")
        {
            for(var i=0;i<4;i++)
                for(var j=0;j<4;j++)
                    spaceSide_jq.find(".Matrix_"+ i +""+ j).val( (i*4 + j < values.length) ? values[i*4 + j] : this.iddentityMatrix4x4_values[i*4 + j] );
            
        }else if(type=="PRS_3d"){              //Pos Rot Scale

            for(var i=0;i<3;i++)
            {
                var prefix = "";
                var nbComposant = 4;
                switch(i)
                {
                    case 0: prefix = "Pos"; break;
                    case 1: prefix = "Rot"; nbComposant = 3; break;
                    case 2: prefix = "Scl"; break;
                }

                for(var j=0;j<nbComposant;j++)
                {
                    var sufix = "";
                    switch(j)
                    {
                        case 0: sufix = "x"; break;
                        case 1: sufix = "y"; break;
                        case 2: sufix = "z"; break;
                        case 3: sufix = "w"; break;
                    }
                    spaceSide_jq.find(".Transform_"+ prefix +"_"+ sufix).val( (i*3 + j < values.length) ? values[i*3 + j] : this.iddentityTransformPRS_values[i*3 + j] );
                }
            }

            this.updateQuatFromRot(isAbsolute);
            
        }else if(type=="POS_3d"){              //Pos Orient (Quaternion) Scale 

            var inc = 0;
            for(var i=0;i<3;i++)
            {
                var prefix = "";
                switch(i)
                {
                    case 0: prefix = "Pos"; break;
                    case 1: prefix = "Quat"; break;
                    case 2: prefix = "Scl"; break;
                }

                for(var j=0;j<4;j++)
                {
                    var sufix = "";
                    switch(j)
                    {
                        case 0: sufix = "x"; break;
                        case 1: sufix = "y"; break;
                        case 2: sufix = "z"; break;
                        case 3: sufix = "w"; break;
                    }
                    spaceSide_jq.find(".Transform_"+ prefix +"_"+ sufix).val( (inc < values.length) ? values[inc] : this.iddentityTransformPOS_values[inc] );
                    inc++;
                }
            }

            this.updateRotFromQuat(isAbsolute);
        }
    }



    /*******************************************************************************
    *                                   getValues                                  *
    *******************************************************************************/
    getValues(type, isAbsolute)
    {
        var values = new Array();

        var spaceSide_jq = $('#Matrix_'+ this.id).find( (isAbsolute) ? ".Matrix_body_abs" : ".Matrix_body_rel");
        if(type=="Matrix4x4")
        {
            for(var i=0;i<4;i++)
                for(var j=0;j<4;j++)
                    values.push( Number(spaceSide_jq.find(".Matrix_"+ i +""+ j).val()) );
            
        }else if(type=="PRS_3d"){              //Pos Rot Scale

            for(var i=0;i<3;i++)
            {
                var prefix = "";
                var nbComposant = 4;
                switch(i)
                {
                    case 0: prefix = "Pos"; break;
                    case 1: prefix = "Rot"; nbComposant = 3; break;
                    case 2: prefix = "Scl"; break;
                }

                for(var j=0;j<nbComposant;j++)
                {
                    var sufix = "";
                    switch(j)
                    {
                        case 0: sufix = "x"; break;
                        case 1: sufix = "y"; break;
                        case 2: sufix = "z"; break;
                        case 3: sufix = "w"; break;
                    }
                    values.push( Number(spaceSide_jq.find(".Transform_"+ prefix +"_"+ sufix).val()) );
                }
            }
            
        }else if(type=="POS_3d"){              //Pos Orient (Quaternion) Scale 

            for(var i=0;i<3;i++)
            {
                var prefix = "";
                switch(i)
                {
                    case 0: prefix = "Pos"; break;
                    case 1: prefix = "Quat"; break;
                    case 2: prefix = "Scl"; break;
                }

                for(var j=0;j<4;j++)
                {
                    var sufix = "";
                    switch(j)
                    {
                        case 0: sufix = "x"; break;
                        case 1: sufix = "y"; break;
                        case 2: sufix = "z"; break;
                        case 3: sufix = "w"; break;
                    }
                    values.push( Number(spaceSide_jq.find(".Transform_"+ prefix +"_"+ sufix).val()) );
                }
            }
        }

        return values;
    }




    /*******************************************************************************
    *                                   setRotValues                               *
    *******************************************************************************/
    setRotValues(values, isAbsolute)
    {
        var spaceSide_jq = $('#Matrix_'+ this.id).find( (isAbsolute) ? ".Matrix_body_abs" : ".Matrix_body_rel");
        
        var prefix = "Rot";
        for(var j=0;j<3;j++)
        {
            var sufix = "";
            switch(j)
            {
                case 0: sufix = "x"; break;
                case 1: sufix = "y"; break;
                case 2: sufix = "z"; break;
            }
            spaceSide_jq.find(".Transform_"+ prefix +"_"+ sufix).val( (j < values.length) ? values[j] : this.iddentityTransformPRS_values[3 + j] );
        }
    }
    /*******************************************************************************
    *                                   getRotValues                               *
    *******************************************************************************/
    getRotValues(isAbsolute)
    {
        var values = new Array();

        var spaceSide_jq = $('#Matrix_'+ this.id).find( (isAbsolute) ? ".Matrix_body_abs" : ".Matrix_body_rel");
        
        var prefix = "Rot";
        for(var j=0;j<3;j++)
        {
            var sufix = "";
            switch(j)
            {
                case 0: sufix = "x"; break;
                case 1: sufix = "y"; break;
                case 2: sufix = "z"; break;
            }
            values.push( Number(spaceSide_jq.find(".Transform_"+ prefix +"_"+ sufix).val()) );
        }
        return values;
    }


    /*******************************************************************************
    *                                   setQuatValues                              *
    *******************************************************************************/
    setQuatValues(values, isAbsolute)
    {
        var spaceSide_jq = $('#Matrix_'+ this.id).find( (isAbsolute) ? ".Matrix_body_abs" : ".Matrix_body_rel");
        
        var prefix = "Quat";
        for(var j=0;j<4;j++)
        {
            var sufix = "";
            switch(j)
            {
                case 0: sufix = "x"; break;
                case 1: sufix = "y"; break;
                case 2: sufix = "z"; break;
                case 3: sufix = "w"; break;
            }
            spaceSide_jq.find(".Transform_"+ prefix +"_"+ sufix).val( (j < values.length) ? values[j] : this.iddentityTransformPOS_values[3 + j] );
        }
    }
    /*******************************************************************************
    *                                   getQuatValues                              *
    *******************************************************************************/
    getQuatValues(isAbsolute)
    {
        var values = new Array();

        var spaceSide_jq = $('#Matrix_'+ this.id).find( (isAbsolute) ? ".Matrix_body_abs" : ".Matrix_body_rel");
        
        var prefix = "Quat";
        for(var j=0;j<4;j++)
        {
            var sufix = "";
            switch(j)
            {
                case 0: sufix = "x"; break;
                case 1: sufix = "y"; break;
                case 2: sufix = "z"; break;
                case 3: sufix = "w"; break;
            }
            values.push( Number(spaceSide_jq.find(".Transform_"+ prefix +"_"+ sufix).val()) );
        }
        return values;
    }




}









