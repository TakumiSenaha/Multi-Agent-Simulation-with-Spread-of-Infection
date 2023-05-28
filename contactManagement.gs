function Seach_Contact() {
  let activesheet = SpreadsheetApp.getActiveSpreadsheet();
  let ss = SpreadsheetApp.openById('1b24l_M219QNSQ3pFPO4h1QMXH-_RZP33ZmQHuTVMNt4');
  let sheet = ss.getSheetByName(activesheet.getSheetName());
  //console.log(activesheet.getSheetName());
  let i;
  let row;
  let contacted_agent = "";
  let ui = SpreadsheetApp.getUi();

  let agent_ID
  agent_ID = Number (Browser.inputBox("Agent_ID...",Browser.Buttons.OK_CANCEL));

  row = Number (agent_ID + 1);
  i = 2;
  while(!sheet.getRange(row,i).isBlank()){
    contacted_agent += sheet.getRange(row,i).getValue() + ", ";
    //console.log(sheet.getRange(row,i).getValue());
    i += 1;
  }
  Browser.msgBox("感染源であると思われるAgent_ID...\n" + contacted_agent);
}
