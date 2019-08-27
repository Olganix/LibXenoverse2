/**
 * Copyright (c) Olganix; Licensed
 */

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// Skeleton //////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

var Skeleton_UNIQUE_ID = 0;

class Skeleton
{
    constructor(divParent, handlerParent)
    {
        if((divParent==undefined)||(divParent==null))
            divParent = "body";
    
        this.id = Skeleton_UNIQUE_ID;
        Skeleton_UNIQUE_ID++;

        this.name = 'Skeleton '+ this.id;
        this.divParent = divParent;
        this.handlerParent = handlerParent;
        this.listBones = new Array();
        

        
        $(divParent).append('\
<div id="Skeleton_'+ this.id +'" class="Skeleton_Panel">\
    <div class="Skeleton_header">\
        <div class="Skeleton_header_top">\
            <div class="Skeleton_header_top_name">'+ this.name +'</div>\
            <div class="Skeleton_header_top_hide"></div>\
            <div class="Skeleton_header_top_delete">X</div>\
        </div>\
        <div class="Skeleton_header_middle">\
            <div class="bt_fillXml">Fill</div>\
            <div class="bt_add bt">+</div>\
            <div class="bt_remove bt active">-</div>\
            <div class="bt_clone bt">c</div>\
        </div>\
    </div>\
    <div class="Skeleton_body">\
        <div class="Skeleton_body_left">\
            <input id="ip_bones_search" placeholder="Filter..." autocomplete="off">\
            <button id="bt_bones_resetSearch" name="reset search" >×</button>\
            <span id="tx_bones_matches"></span>\
            <div id="tn_bones" ></div>\
        </div>\
        <div class="Skeleton_body_right">\
        </div>\
    </div>\
</div>\
'); 
        var main_jq = $('#Skeleton_'+ this.id);

        

        
        var context = this;                         //for actions
        

        main_jq.find(".Skeleton_header_top_hide").click(function()
        {
            var list = $(this).parent().parent().parent().find(".Skeleton_body, .Skeleton_header_middle");
            
            $(this).toggleClass("active");
            
            if($(this).hasClass("active"))
                list.addClass("hide");
            else
                list.removeClass("hide");
        });

        main_jq.find(".Skeleton_header_top_delete").click(function()
        {
            if(ckv(context.boneInstance))
                context.boneInstance.notifySkeletonRemoved(context);
            
            if(ckv(context.handlerParent))
                context.handlerParent.notifySkeletonRemoved(context);
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
    <textarea class="ta_fill" name="fast fill Skeleton with Xml" placeholder="put some esk Xml here." value="" ></textarea>\
    <div class="bt_Esk_Xml bt">Get Esk</div>\
    <div class="bt_fill_Skeleton bt">Fill Skeleton</div>\
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

                jq_div.find(".bt_fill_Skeleton").click(function()
                {
                    var xml_str = jq_div.find(".ta_fill").val();
                    if((xml_str.indexOf("F_00")!=-1)&&(xml_str.indexOf("F_04")!=-1)&&(xml_str.indexOf("F_08")!=-1)&&(xml_str.indexOf("F_12")!=-1)&&(xml_str.indexOf("F_16")!=-1)&&(xml_str.indexOf("F_20")!=-1)&&(xml_str.indexOf("F_24")!=-1)&&(xml_str.indexOf("F_28")!=-1)&&(xml_str.indexOf("F_32")!=-1)&&(xml_str.indexOf("F_36")!=-1)&&(xml_str.indexOf("F_40")!=-1)&&(xml_str.indexOf("F_44")!=-1)&&(xml_str.indexOf("F_48")!=-1)&&(xml_str.indexOf("F_52")!=-1)&&(xml_str.indexOf("F_56")!=-1)&&(xml_str.indexOf("F_60")!=-1))
                    {                                               //it's Transform from Lazybone Fpf Xml.
                        var jq_doc = $(xml_str);
                        var values = new Array();
                        for(var i=0;i<=60;i+=4)
                            values.push(Number( jq_doc.find("F_"+ numberTostring(i, 2) ).attr("value") ));

                        context.setValues(values, "Skeleton4x4", false);
                        context.updateSkeleton(false);
                        context.updateTransformFromSkeleton(true);
                        context.updateTransformFromSkeleton(false);




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
                        context.updateSkeletonFromTransform(false);    
                        context.updateSkeleton(false);
                        context.updateTransformFromSkeleton(true);




                    }else if((xml_str.indexOf("Line")!=-1)){
                                                                    //it's ESK AbsoluteTransformSkeleton

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
                        context.setValues(values, "Skeleton4x4", true);
                        context.updateSkeleton(true);
                        context.updateTransformFromSkeleton(true);
                        context.updateTransformFromSkeleton(false);
                        

                    }else{
                        alert("Error: xml is not understable. try Esk RelativeTransform, Esk AbsoluteTransformSkeleton, or fpf Transform.")
                    }
                });



                jq_div.find(".bt_Esk_Xml").click(function()
                {
                    var values = context.getValues("Skeleton4x4", true);

                    var str = "<AbsoluteTransformSkeleton>\n";
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
                    str += "</AbsoluteTransformSkeleton>\n";

                    jq_div.find(".ta_fill").val(str);
                });

            }else{
                jq_div.find(".fill_panel").remove();
            }
        });

    }

    destroy()
    {
        $('#Skeleton_'+ this.id).remove();
        this.listBones = new Array();
    }


    /*

    //objects's categories
        if( (!$('#ta_objCatList').hasClass("hide")) && (ckv(this.infos.categoriesObjects)))
        {
            var arboObjCat_str = JSON.stringify(this.infos.categoriesObjects);
            arboObjCat_str = arboObjCat_str.replaceAll('"child"', '"children"').replaceAll('"name"', '"title"').replaceAll('"id"', '"key"').replaceAll('"unlocked":true', '"type":"dlc_allowed"').replaceAll('"unlocked":false', '"type":"dlc_notAllowed"');
            var arboObjCat = JSON.parse(arboObjCat_str);

            //arboObjCat.push( {title: "Geographie", children: [{title: "France"}, {title: "Europe", type:"dlc_notAllowed", children: [ {title: "démographie", type:"dlc_allowed"}, {title: "faits divers", type:"dlc_notAllowed"} ]}] } );       //test Todo remove.

            $('#ta_objCatList').addClass("hide");

            $('#ta_objCatList').after('\
            <input id="ip_objCatList_search" placeholder="Filter..." autocomplete="off">\
            <button id="bt_objCatList_resetSearch" name="reset search" >×</button>\
            <span id="tx_objCatList_matches"></span>\
            <div id="tn_bones" ></div>\
            ');
            
            $("#tn_bones").fancytree({
                extensions: ['edit', 'filter'],
                selectMode: 3,                  //(De)selecting a node will propagate to all descendants. Mixed states will be displayed as partially selected using a tri-state checkbox
                quicksearch: true,
                filter: {
                    autoExpand: true,
                    highlight: true, 
                    leavesOnly: true,
                    nodata: true
                },
                types: 
                {
                    "dlc_notAllowed": {icon: "padlock_closed", checkbox: false, iconTooltip: "Contenu supplémentaire n'est pas débloqué"},
                    "dlc_allowed":    {icon: "padlock_open", iconTooltip: "Contenu supplémentaire débloqué"},
                },
                checkbox: function(event, data)
                {
                    return (data.node.type!="dlc_notAllowed");
                },
                icon: function(event, data)
                {
                    if(data.node.type=="dlc_notAllowed")
                        return "padlock_closed";
                    if(data.node.type=="dlc_allowed")
                        return "padlock_open";
                    return false;
                },

                source: arboObjCat,

                select: function(event, data)
                {
                    if(!context.treeUpdateing)
                        context.updateObjectCategoriesSelected();
                },
                click: function(event, data) 
                {
                    if(((data.targetType=="title")||(data.targetType=="icon"))&&(data.node.type=="dlc_notAllowed"))
                        window.open('dlc.php');
                }
            });            

            $("#ip_objCatList_search").keyup(function(e)
            {
                var text = $(this).val();
                if(e && e.which === $.ui.keyCode.ESCAPE || $.trim(text) === "")
                {
                    $("#bt_objCatList_resetSearch").click();
                    return;
                }
            
                var nbMatch = $("#tn_bones").fancytree("getTree").filterBranches(text);    
                
                $("#bt_objCatList_resetSearch").attr("disabled", false);
                $("#tx_objCatList_matches").text("(" + nbMatch + " trouvé(es) )");
            }).focus();
    
            $("#bt_objCatList_resetSearch").click(function(e)
            {
                $("#ip_objCatList_search").val("");
                $("#tx_objCatList_matches").text("");
                $("#tn_bones").fancytree("getTree").clearFilter();
            }).attr("disabled", true);


            $("#tn_bones").find(".fancytree-container").addClass("fancytree-connectors");
        }

    */

}









