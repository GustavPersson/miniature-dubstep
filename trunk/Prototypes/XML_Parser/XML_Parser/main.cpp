#include "tinyxml.h"
//#include "stdafx.h"


void load_file();
const char *getIndent(unsigned int numIndents);
void dump_to_stdout(TiXmlNode *pParent, unsigned int indent = 0);
void write_simple_doc();
void write_simple_doc2();
void write_simple_doc3();
void load_and_display();
int dump_attribs_to_stdout(TiXmlElement *pElement, unsigned int indent);
void write_app_settings_doc();

int main()
{
	load_file();
	return 0;
}

void load_file( )
{
    TiXmlDocument doc( "HelloWorld.xml" );
    bool loadOkay = doc.LoadFile();

    if ( loadOkay )
    {
		TiXmlElement *test = new TiXmlElement("test");
		test->SetAttribute("name", "Circle");
		test->SetAttribute("x", 5);
		test->SetAttribute("y", 15);
		test->SetDoubleAttribute("radius", 3.14159);
        // Your document is loaded - do what you like
        // with it.
        //
        // Here we'll dump the structure to STDOUT,
        // just for example
        dump_to_stdout( &doc );
		printf("\n\n\n");
		write_simple_doc();
		printf("\n\n\n");
		write_simple_doc2();
		printf("\n\n\n");
		write_simple_doc3();
		printf("\n\n\n");
		load_and_display();
		printf("\n\n\n");
		dump_attribs_to_stdout(test, 0);
		printf("\n\n\n");
		write_app_settings_doc();
    }
    else
    {
        // load failed
    }
}

// a utility function defining a very simple method to indent a line of text
const char * getIndent( unsigned int numIndents )
{
    static const char * pINDENT = "                                      + ";
    static const unsigned int LENGTH = strlen( pINDENT );

    if ( numIndents > LENGTH ) numIndents = LENGTH;

    return &pINDENT[ LENGTH-numIndents ];
}

void dump_to_stdout( TiXmlNode * pParent, unsigned int indent )
{
    if ( !pParent ) return;

    TiXmlText *pText;
    int t = pParent->Type();
    printf( "%s", getIndent( indent));

    switch ( t )
    {
    case TiXmlNode::DOCUMENT:
        printf( "Document" );
        break;

    case TiXmlNode::ELEMENT:
        printf( "Element \"%s\"", pParent->Value() );
        break;

    case TiXmlNode::COMMENT:
        printf( "Comment: \"%s\"", pParent->Value());
        break;

    case TiXmlNode::UNKNOWN:
        printf( "Unknown" );
        break;

    case TiXmlNode::TEXT:
        pText = pParent->ToText();
        printf( "Text: [%s]", pText->Value() );
        break;

    case TiXmlNode::DECLARATION:
        printf( "Declaration" );
        break;
    default:
        break;
    }
    printf( "\n" );

    TiXmlNode * pChild;

    for ( pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
    {
        dump_to_stdout( pChild, indent+2 );
    }
}

void write_simple_doc( )
{
    // Make xml: <?xml ..><Hello>World</Hello>
    TiXmlDocument doc;
    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    TiXmlElement * element = new TiXmlElement( "Hello" );
    TiXmlText * text = new TiXmlText( "World" );
    element->LinkEndChild( text );
    doc.LinkEndChild( decl );
    doc.LinkEndChild( element );
    
    dump_to_stdout( &doc );
    doc.SaveFile( "madeByHand.xml" );
}

void write_simple_doc2( )
{
    // same as write_simple_doc1 but add each node
    // as early as possible into the tree.

    TiXmlDocument doc;
    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    doc.LinkEndChild( decl );
    
    TiXmlElement * element = new TiXmlElement( "Hello" );
    doc.LinkEndChild( element );
    
    TiXmlText * text = new TiXmlText( "World" );
    element->LinkEndChild( text );
    
    dump_to_stdout( &doc );
    doc.SaveFile( "madeByHand2.xml" );
}

void write_simple_doc3( )  
{  
    // This example courtesy of polocolege
 
    TiXmlDocument doc;  
    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );  
    doc.LinkEndChild( decl );  
 
    TiXmlElement * element = new TiXmlElement( "Hello" );  
    doc.LinkEndChild( element );  
 
    TiXmlText * text = new TiXmlText( "Opening a new salutation" );  
    element->LinkEndChild( text );  
 
    TiXmlElement * element2 = new TiXmlElement( "Greeting" );  
    element->LinkEndChild( element2 );  
 
    TiXmlText * text2 = new TiXmlText( "How are you?" );  
    element2->LinkEndChild( text2 );  
 
    TiXmlElement * element3 = new TiXmlElement( "Language" );  
    element2->LinkEndChild( element3 );  
 
    TiXmlText * text3 = new TiXmlText( "English" );  
    element3->LinkEndChild( text3 );  
 
    TiXmlElement * element4 = new TiXmlElement( "Exclamation" );  
    element->LinkEndChild( element4 );  
 
    TiXmlText * text4 = new TiXmlText( "You have children!" );  
    element4->LinkEndChild( text4 );  
 
    dump_to_stdout( &doc );
    doc.SaveFile( "madeByHand3.xml" );  
}  

void load_and_display( )
{
    // important for the poetry demo, but you may not need this 
    // in your own projects
    TiXmlBase::SetCondenseWhiteSpace( false );

    TiXmlDocument doc( "App.xml" );
    bool loadOkay = doc.LoadFile();

    if ( loadOkay )
    {
        dump_to_stdout( &doc );
    }
    else
    {
        printf( "Something went wrong\n" );
    }
}

// print all attributes of pElement.
// returns the number of attributes printed
int dump_attribs_to_stdout(TiXmlElement* pElement, unsigned int indent)
{
	if ( !pElement ) return 0;

	TiXmlAttribute* pAttrib=pElement->FirstAttribute();
	int i=0;
	int ival;
	double dval;
	const char* pIndent=getIndent(indent);
	printf("\n");
	while (pAttrib)
	{
		printf( "%s%s: value=[%s]", pIndent, pAttrib->Name(), pAttrib->Value());

		if (pAttrib->QueryIntValue(&ival)==TIXML_SUCCESS)    printf( " int=%d", ival);
		if (pAttrib->QueryDoubleValue(&dval)==TIXML_SUCCESS) printf( " d=%1.1f", dval);
		printf( "\n" );
		i++;
		pAttrib=pAttrib->Next();
	}
	return i;
}

void write_app_settings_doc( )  
{  
	TiXmlDocument doc;  
	TiXmlElement* msg;
 	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
	doc.LinkEndChild( decl );  
 
	TiXmlElement * root = new TiXmlElement( "MyApp" );  
	doc.LinkEndChild( root );  

	TiXmlComment * comment = new TiXmlComment();
	comment->SetValue(" Settings for MyApp " );  
	root->LinkEndChild( comment );  
 
	TiXmlElement * msgs = new TiXmlElement( "Messages" );  
	root->LinkEndChild( msgs );  
 
	msg = new TiXmlElement( "Welcome" );  
	msg->LinkEndChild( new TiXmlText( "Welcome to MyApp" ));  
	msgs->LinkEndChild( msg );  
 
	msg = new TiXmlElement( "Farewell" );  
	msg->LinkEndChild( new TiXmlText( "Thank you for using MyApp" ));  
	msgs->LinkEndChild( msg );  
 
	TiXmlElement * windows = new TiXmlElement( "Windows" );  
	root->LinkEndChild( windows );  

	TiXmlElement * window;
	window = new TiXmlElement( "Window" );  
	windows->LinkEndChild( window );  
	window->SetAttribute("name", "MainFrame");
	window->SetAttribute("x", 5);
	window->SetAttribute("y", 15);
	window->SetAttribute("w", 400);
	window->SetAttribute("h", 250);

	TiXmlElement * cxn = new TiXmlElement( "Connection" );  
	root->LinkEndChild( cxn );  
	cxn->SetAttribute("ip", "192.168.0.1");
	cxn->SetDoubleAttribute("timeout", 123.456); // floating point attrib
	
	dump_to_stdout( &doc );
	doc.SaveFile( "appsettings.xml" );  
} 
