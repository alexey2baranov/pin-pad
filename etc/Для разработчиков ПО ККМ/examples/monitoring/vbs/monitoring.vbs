Dim MyServ
Dim MState, MType
Dim MMessage

Set MyServ = CreateObject("SBRFSRV.Server")

MyServ.Clear
MyServ.SParam "CmdLine", "C:\WORK\upos\sbrf.dll 47 2"
Err = MyServ.NFun(17)
If (err=0) then
  MState = MyServ.GParamString("MonHostState")
  MType  = MyServ.GParamString("MonMessageType")
  MMessage = MyServ.GParamString("MonMessage1251")
  SMessage="Состояние: " & MState & Chr(13)
  SMessage=SMessage & "Тип сообщения: " & MType  & Chr(13)
  SMessage=SMessage & "Соообщение: " & MMessage  
  MsgBox SMessage, vbOkOnly+vbInformation, "Сообщение системы мониторинга"
  showTextInNotepad(MyServ.GParamString("Cheque1251"))
End if

MyServ.Clear
Set MyServer = Nothing


sub showTextInNotepad(text)
filename = date & "upos_check.txt"
set fso = CreateObject("Scripting.FileSystemObject")
set file = fso.CreateTextFile(filename)
file.Write(text)
file.Close()
CreateObject("WScript.Shell").Run("notepad " + filename)
WScript.Sleep(2000)
fso.DeleteFile(filename)
end sub