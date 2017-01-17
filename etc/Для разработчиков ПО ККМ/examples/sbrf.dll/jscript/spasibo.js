var resultCode
var bankCheck
var rrn
var mHash


oleObject = new ActiveXObject("SBRFSRV.Server")
ws = WScript.CreateObject('WScript.Shell')

oleObject.clear()
oleObject.SParam ("Amount", 100000)
resultCode = oleObject.NFun(4000)
mHash = oleObject.GParamString("HASH")
if (resultCode == 4353)
{
  oleObject.SParam ("Amount", 90000)
  oleObject.SParam ("INHASH", mHash)
  resultCode = oleObject.NFun(4000)
}

bankCheck = oleObject.GParamString("Cheque1251")
resultCheck = "ERRCODE: " + resultCode + "\r\n" + "HASH: " + mHash + "\r\n\r\n" + bankCheck
showTextInNotepad(resultCheck)
oleObject.clear()

function showTextInNotepad(text){
filename = (new Date()).valueOf() + "upos_check.txt"
fso = WScript.CreateObject('Scripting.FileSystemObject')
file = fso.CreateTextFile(filename)
file.Write(text)
file.Close()
WScript.CreateObject('WScript.Shell').Run("notepad " + filename)
WScript.Sleep(2000)
fso.DeleteFile(filename)
}