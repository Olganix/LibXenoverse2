


////////////////////////////////////////////////////////////

var listCustomMatrix = new Array();

$(document).ready(function()
{
    
    $(".list_matrix").sortable({
        connectWith: ".list_matrix",
        items: '.Matrix_Panel',
        dropOnEmpty: true
    }).disableSelection();


    $(".bt_add_Matrix").click(function()
    {
        addEmptyMatrix();
    });
});






/*******************************************************************************
*                                   addEmptyMatrix                             *
*******************************************************************************/
function addEmptyMatrix()
{
    listCustomMatrix.push( new Matrix(".list_matrix.firstlist", this, null) );
}

/*******************************************************************************
*                                   clear                                      *
*******************************************************************************/
function clear()
{
    for(var i=0,nb=listCustomMatrix.length;i<nb;i++)
    {
        listCustomMatrix[i].clear();
        delete listCustomMatrix[i];
    }
    listCustomMatrix.clear();
}

/*******************************************************************************
*                                   notifyMatrixRemoved                        *
*******************************************************************************/
function notifyMatrixRemoved(matrixToDelete)
{
    for(var i=0,nb=listCustomMatrix.length;i<nb;i++)
    {
        if(listCustomMatrix[i].name == matrixToDelete.name)
        {
            listCustomMatrix.splice(i,1);
            break;
        }
    }
    matrixToDelete.destroy();
    delete matrixToDelete;
}
















/*******************************************************************************
*                                   import_ExkXml                              *
*******************************************************************************/
function import_ExkXml(str_xml)
{
    //todo  hierarchy.
    /*
    if( (!$('#ta_objCatList').hasClass("hide")) && (ckv(this.infos.categoriesObjects)))
    {
        var arboObjCat_str = JSON.stringify(this.infos.categoriesObjects);
        arboObjCat_str = arboObjCat_str.replaceAll('"child"', '"children"').replaceAll('"displayName"', '"title"').replaceAll('"id"', '"key"').replaceAll('"unlocked":true', '"type":"dlc_allowed"').replaceAll('"unlocked":false', '"type":"dlc_notAllowed"');
        var arboObjCat = JSON.parse(arboObjCat_str);

        //arboObjCat.push( {title: "Geographie", children: [{title: "France"}, {title: "Europe", type:"dlc_notAllowed", children: [ {title: "démographie", type:"dlc_allowed"}, {title: "faits divers", type:"dlc_notAllowed"} ]}] } );       //test Todo remove.


        $('#ta_objCatList').addClass("hide");

        $('#ta_objCatList').after('\
        <input id="ip_objCatList_search" type="text" placeholder="Filter..." autocomplete="off" onFocus="'+ ((!__isTouchDevice) ? 'this.setSelectionRange(0, this.value.length);' : 'this.select();') +'" >\
        <button id="bt_objCatList_resetSearch" name="reset search" >×</button>\
        <span id="tx_objCatList_matches"></span>\
        <div id="tn_objCatList" ></div>\
        ');
        
        $("#tn_objCatList").fancytree({
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
                    goOnUrl(__baseUrl_main, true);          //Todo dlc direct
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
        
            var nbMatch = $("#tn_objCatList").fancytree("getTree").filterBranches(text);    
            
            $("#bt_objCatList_resetSearch").attr("disabled", false);
            $("#tx_objCatList_matches").text("(" + nbMatch + " trouvé(es) )");
        });

        $("#bt_objCatList_resetSearch").click(function(e)
        {
            $("#ip_objCatList_search").val("");
            $("#tx_objCatList_matches").text("");
            $("#tn_objCatList").fancytree("getTree").clearFilter();
        }).attr("disabled", true);


        $("#tn_objCatList").find(".fancytree-container").addClass("fancytree-connectors");
    }else{
        $('#ta_objCatList').addClass("hide");
    }
    */
}
