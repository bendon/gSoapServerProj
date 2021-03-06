var soap_req;
function getUrl(path)
{
    u = document.URL;
    if (u.indexOf('?') > 0)
    {
        u = u.substring(0, u.indexOf('?'));
    }
    return u.substring(0, u.lastIndexOf('/')) + '/' + path;
}
function tbl_last_row()
{
    var rs = document.getElementById('config').rows;
    if (rs.length > 0)
    {
        return $(rs[rs.length - 1]).clone();
    }
    return null;
    // error
}
function done() {
    b = true;
    $('table').find('td').each(function () {
        txt = $(this).text()
        if (txt == $('#txtKey').attr("value"))
        {
            b = false;
            $(this).parent().children().each(function () {
                if (this.cellIndex == 2)
                {
                    $(this).text($('#txtVal').attr("value"));
                }
            });
        }
    });
    if (b)
    {
        mo(tbl_last_row().clone()).appendTo($('table')).children().each(function () {
            idx = this.cellIndex;
            switch (idx)
            {
                case 0:
                    $(this).text($('#txtKey').attr('value'));
                    break;
                case 2:
                    $(this).text($('#txtVal').attr('value'));
                    break;
                case 3:
                    $(this).find('input').attr('id', $(this).parent()[0].rowIndex).click(update);
                    break;
                default:
                    break;
            }
        });
    }
    $('#btnClose').click();
}
function update() {
    t = (document.body.clientHeight - $('#dialog').height()) / 2;
    l = (document.body.clientWidth - $('#dialog').width()) / 2;
    if (t < 0) t = 0;
    if (l < 0) l = 0;
    k = $(document.getElementById('config').rows).get(this.id).cells[0];
    v = $(document.getElementById('config').rows).get(this.id).cells[2];
    $('#dialog').css({ bottom: t, right: l });
    $('#dialog').show('slow', function() {
        $('#txtKey').attr("value", $(k).text());
        $('#txtVal').attr("value", $(v).text()).focus();
    });
    $('#lblInfo').text("");
}
$(document).ready(function() {
    $('#btn').click(function() {
        $('#lblInfo').text("发送请求... ...");
        jQuery.ajax({
            type: "POST", //"GET",
            url: document.URL,
            data: soap_req,
            dataType: "xml",
            success: function(result) {
                $('#lblInfo').text("完成...");
                $(result).find('result').each(function() {
                    if ($(this).text() == "true")
                    {
                        $('#lblInfo').text("成功....");
                        setTimeout("done()", 768);
                    }
                    else
                    {
                        $('#lblInfo').text("失败。");
                        setTimeout("$('#btnClose').click()", 768);
                    }
                });
            },
            error: function(result) {
                $('#lblInfo').text($(result.responseXML).find('faultstring').text());
            }
        });
    });
});
$(document).ready(function() {
    $('#btnGetRequestSoapData').click(function() {
        $('#lblInfo').text("发送请求...");
        jQuery.ajax({
            type: "GET", //"POST", 
            url: getUrl("getsoapdata"),
            contentType: "application/soap",
            success: function(result) {
                soap_req = result;
                typeof soap_req;
                if (typeof(soap_req) == "object")
                {
                    $(soap_req).find('key').text($('#txtKey').attr("value"));
                    $(soap_req).find('value').text($('#txtVal').attr("value"));
                    $('#btn').click();
                }
                if (typeof(soap_req) == "string")
                {
                    soap_req = soap_req.replace('<key></key>', '<key>' + $('#txtKey').attr("value") + '</key>');
                    soap_req = soap_req.replace('<value></value>', '<value>' + $('#txtVal').attr("value") + '</value>');
                    $('#btn').click();
                }
            }
        });
    });
});
$(document).ready(function() {
    $('#btnClose').click(function() {
        $('#dialog').hide('slow');
    });
    $('#btnModify').click(function() {
        //$('#btnClose').click();
        $('#btnGetRequestSoapData').click();
    });
});
$(document).ready(function() {
    $('#btnGetTable').click(function() {
        $(document.getElementById('config').rows).each(function() {
            input = $(this).append('<td><input type="button" value="修改" id="' + this.rowIndex + '" /></td>').find('input');
            input.click(update);
            mo($(this));
        });
    });
    $('#btnGetTable').click();
});
function mo(jqRow) {
    jqRow.mouseover(function(event) {
        e = jqRow.css('background-color', 'Aqua');
        e = event.toString();
    })
    .mouseout(function(event) {
        e = event.toString();
        jqRow.css('background-color', 'transparent');
    });
    return jqRow;
}