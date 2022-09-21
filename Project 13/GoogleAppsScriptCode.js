function doGet(e) {
    Logger.log(JSON.stringify(e));
    var result = "Failed receiving Parameters";

    if (e == null || e.parameter == null) {
        result = "No Parameters";
    }
    else {
        var GSheetID = "YOUR GOOGLE SPREADSHEET ID";
        var LM35DataSheet = SpreadsheetApp.openById(GSheetID).getSheetByName("LM35Data");
        var newRow = LM35DataSheet.getLastRow() + 1;
        var rowData = [];

        var currDate = new Date();
        rowData[0] = currDate.toLocaleDateString();
        rowData[1] = currDate.toLocaleTimeString();

        for (var param in e.parameter) {
            var value = (e.parameter[param]).replace(/["']/g, "");

            if (param == "temperature") {
                rowData[2] = value;
                result = "Data written on column C Successfully.";
            } else if (param == "condition") {
                rowData[3] = value;

                if (value == "COLD") {
                    LM35DataSheet.getRange(newRow, 4).setBackground("#66CDAA");
                } else if (value == "NORMAL") {
                    LM35DataSheet.getRange(newRow, 4).setBackground("#7CFC00");
                } else if (value == "HOT") {
                    LM35DataSheet.getRange(newRow, 4).setBackground("#DC143C");
                }

                result += "\nData written on column D Successfully.";
            } else {
                result += "\nUnsupported Parameter Detected.";
            }
        }

        LM35DataSheet.getRange(newRow, 1, 1, rowData.length).setValues([rowData]);
    }
    return ContentService.createTextOutput(result);
}