var SHEET_NAME = "Sheet1";

function doGet(e){
  return HtmlService.createHtmlOutput("Google Sheets Webhook v0.1");
}

function doPost(e){

  var theJSON = JSON.parse(e.postData.contents);

  var theSheet = SpreadsheetApp.getActiveSpreadsheet().getSheetByName(SHEET_NAME);

  var data = [];
  
  var theDate = Utilities.formatDate(new Date(), 'GMT-0', 'dd/MM/yyyy');
  data.push(theDate);

  var decoded = Utilities.base64Decode(theJSON.uplink_message.frm_payload);
  var decoded2 = Utilities.newBlob(decoded).getDataAsString();
  data.push(decoded2);

  theSheet.appendRow(data);

}
