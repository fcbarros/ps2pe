//---------------------------------------------------------------------------

#ifndef UnitPS2H
#define UnitPS2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
class TFormDebug : public TForm
{
__published:	// IDE-managed Components
    TPageControl *PageControlDisassembly;
    TTabSheet *TabDisassembly;
    TStringGrid *GridDisassembly;
    TScrollBar *ScrollBarDisassembly;
    TTabSheet *TabMemory;
    TStringGrid *GridMemory;
    TScrollBar *ScrollBarMemory;
    TTabSheet *TabStats;
    TGroupBox *GroupBoxInstructionsStats;
    TImageList *ImageListToolBar;
    TOpenDialog *OpenProgram;
    TImageList *ImageListRun;
    TPageControl *PageControlInfo;
    TTabSheet *TabSheetGPR;
    TStringGrid *GridGPR1;
    TStringGrid *GridGPR2;
    TTabSheet *TabSheetFPR;
    TTabSheet *TabSheetOthers;
    TTabSheet *TabSheetInfo;
    TGroupBox *GroupBoxInfoAssembly;
    TLabel *LabelSupportedInstructions;
    TLabel *LabelTotalInsructions;
    TGroupBox *GroupBoxInfoProgram;
    TLabel *LabelSizeText;
    TLabel *LabelProgramSize;
    TToolBar *ToolBar1;
    TToolButton *ToolButtonRun;
    TToolButton *ToolButtonPause;
    TToolButton *ToolButtonNext;
    TToolButton *ToolButtonStep;
    TPanel *PanelFind;
    TEdit *EditFind;
    TBitBtn *BitBtn1;
    TPanel *PanelGoTo;
    TEdit *EditGoTo;
    TBitBtn *ButtonGoTo;
    TToolBar *ToolBarGeneral;
    TToolButton *ButtonOpen;
    TGroupBox *GroupBoxElf;
    TRichEdit *MemoElf;
    TStringGrid *GridFPR1;
    TStringGrid *GridFPR2;
    TGroupBox *GroupBox1;
    TStringGrid *GridLoadedStats;
    TGroupBox *GroupBox2;
    TStringGrid *GridInstructionsStats;
    TStringGrid *GridRunningStats;
    TTabSheet *TabSheet1;
    TMemo *MemoSpeed;
    TButton *ButtonBenchmark;
    TStringGrid *GridCOP01;
    TStringGrid *GridCOP02;
    void __fastcall GridDisassemblyKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall GridDisassemblyKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall ScrollBarDisassemblyChange(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall ButtonOpenClick(TObject *Sender);
    void __fastcall GridMemoryKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall GridMemoryKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall ScrollBarMemoryChange(TObject *Sender);
    void __fastcall ButtonGoToClick(TObject *Sender);
    void __fastcall ToolButtonNextClick(TObject *Sender);
    void __fastcall ToolButtonRunClick(TObject *Sender);
    void __fastcall GridDisassemblyMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall ToolButtonStepClick(TObject *Sender);
    void __fastcall ToolButtonPauseClick(TObject *Sender);
    void __fastcall GridInstructionsStatsMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall ButtonBenchmarkClick(TObject *Sender);
private:	// User declarations
    void __fastcall InitializeDisassembly( void );
    void __fastcall Disassembly( void );
    void __fastcall FillPS2Memory( void );
    void __fastcall DisassemblyGo( unsigned int Pos );
    void __fastcall MemoryGo( unsigned int Pos );
    void __fastcall UpdateRegisters( void );
    void __fastcall GenerateStats( void );
    void __fastcall GenerateRunningStats( void );
    void __fastcall GenerateInstructionsStats( void );
    void __fastcall BenchMark( void );

    unsigned int ProgramSize;
    bool Running;
    int OldRow;
    WORD OldKeyMemory;
    WORD OldKeyDisassembly;

public:		// User declarations
    __fastcall TFormDebug(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDebug *FormDebug;
//---------------------------------------------------------------------------
#endif
