#ifndef MTEXTAREA_H
#define MTEXTAREA_H

#pragma warning(disable:4786)

#include <list>
#include <string>
#include "MWidget.h"
#include "MLookNFeel.h"
#include "MScrollBar.h"

using namespace std;

class MTextArea;
class MScrollBar;

#define MTEXTAREA_DEFAULT_TEXT_COLOR		MCOLOR(224,224,224)

class MTextAreaLook {
protected:
	MCOLOR m_bgColor;
public:
	MTextAreaLook() : m_bgColor(0, 0, 0, 0) {}
	virtual void OnDraw(MTextArea* pTextArea, MDrawContext* pDC);
	virtual MRECT GetClientRect(MTextArea* pTextArea, MRECT& r);
	virtual void SetBgColor(MCOLOR& c) { m_bgColor = c; }

private:
	virtual void OnFrameDraw(MTextArea* pTextArea, MDrawContext* pDC);
	virtual void OnTextDraw(MTextArea* pTextArea, MDrawContext* pDC);
	virtual void OnTextDraw_WordWrap(MTextArea* pTextArea, MDrawContext* pDC);
};

struct MLineItem {
	MLineItem(MCOLOR _color, string& _text) {
		color = _color;
		text = _text;
	}
	MLineItem(string& _text) {
		color = MTEXTAREA_DEFAULT_TEXT_COLOR;
		text = _text;
	}
	MCOLOR color;
	string text;
};

typedef list<MLineItem> MLINELIST;
typedef list<MLineItem>::iterator MLINELISTITERATOR;

class MTextArea : public MWidget {
	friend MTextAreaLook;
protected:
	bool		m_bScrollBarEnable;
	int			m_nIndentation;
	bool		m_bWordWrap;
	bool		m_bColorSupport;
	MPOINT		m_TextOffset;
	bool		m_bMouseOver;
	MCOLOR		m_TextColor;
	int			m_nMaxLen;
	char		m_szIMECompositionString[MIMECOMPOSITIONSTRING_LENGTH];
	bool		m_bEditable;
	int			m_nStartLine;
	int			m_nStartLineSkipLine;

	int			m_nCurrentSize;
	bool		m_bVerticalMoving;
	int			m_nVerticalMoveAxis;

	int			m_nCustomLineHeight;

	MPOINT		m_CaretPos;
	bool		m_bCaretFirst;

	MLINELIST			m_Lines;
	MLINELISTITERATOR	m_CurrentLine;

	MScrollBar* m_pScrollBar;

	DECLARE_LOOK(MTextAreaLook)
	DECLARE_LOOK_CLIENT()

protected:
	virtual void OnSize(int w, int h);
	virtual bool OnCommand(MWidget* pWindow, const char* szMessage);
	virtual bool OnEvent(MEvent* pEvent, MListener* pListener);
	virtual void OnSetFocus(void);
	virtual void OnReleaseFocus(void);

	virtual bool InputFilterKey(int nKey, bool bCtrl);
	virtual bool InputFilterChar(int nKey);

	bool OnLButtonDown(MPOINT pos);
	void OnScrollBarChanged(int nPos);

	bool MoveLeft(bool bBackspace = false);
	void MoveRight();
	void MoveDown();
	void MoveUp();
	void DeleteCurrent();
	void ScrollDown();
	void ScrollUp();
	void MoveFirst();
	void MoveLast();
	void OnHome();
	void OnEnd();

	bool GetCaretPosition(MPOINT* pOut, int nSize, const char* szText, int nPos, bool bFirst);

	int GetCharPosition(const char* szText, int nX, int nLine);

	bool IsDoubleCaretPos();

	void UpdateScrollBar(bool bAdjustStart = false);

	MLINELISTITERATOR GetIterator(int nLine);

public:
	MTextArea(int nMaxLen = 120, const char* szName = NULL, MWidget* pParent = NULL, MListener* pListener = NULL);
	virtual ~MTextArea();

#define MINT_TEXTAREA	"TextArea"
	virtual const char* GetClassName(void) { return MINT_TEXTAREA; }

	MPOINT GetCaretPos() { return m_CaretPos; }
	int	GetStartLine() { return m_nStartLine; }

	bool IsScrollBarVisible() { return m_pScrollBar->IsVisible(); }
	int GetScrollBarWidth() { return m_pScrollBar->GetRect().w; }
	int GetScrollBarMax() { return m_pScrollBar->GetMax(); }

	int GetClientWidth();

	int GetLength() { return (int)(m_nCurrentSize + m_Lines.size()); }
	int GetLineCount() { return (int)m_Lines.size(); }
	int GetTotalLineCount(int& nStartLine, int& nCurrentLine);

	bool GetText(char* pBuffer, int nBufferSize);
	const char* GetTextLine(int nLine);

	void SetMaxLen(int nMaxLen);
	int	GetMaxLen() { return m_nMaxLen; }

	const char* GetCompositionString(void);

	void SetEditable(bool editable) { m_bEditable = editable; }
	bool GetEditable() { return m_bEditable; }

	void SetScrollBarEnable(bool bEnable) { m_bScrollBarEnable = bEnable; }
	bool GetScrollBarEnable() { return m_bScrollBarEnable; }

	void SetTextOffset(MPOINT p);

	void SetIndentation(int nIndentation) { m_nIndentation = nIndentation; }

	void SetTextColor(MCOLOR color);
	MCOLOR GetTextColor(void);

	void Clear();

	void SetText(const char* szText);

	void AddText(const char* szText, MCOLOR color);
	void AddText(const char* szText);

	void DeleteFirstLine();

	int GetLineHeight(void);
	void SetCustomLineHeight(int nHeight);

	virtual void MultiplySize(float byIDLWidth, float byIDLHeight, float byCurrWidth, float byCurrHeight);

	void AdjustHeightByContent();

	//Custom: GetTextPosition for emojis
	int GetTextPosition(const char* text);
};

#define MTEXTAREA_ENTER_VALUE		"entered"
#define MTEXTAREA_ESC_VALUE			"esc"

#endif