/**
 @file macOSAlert.mm
 @date 12.02.15
 @copyright Cadabra Systems / Abra
 @author Daniil A Megrabyan
 */

#include "../Alert.hpp"

#include "InputAlert.hpp"
#include "ActionAlert.hpp"

#include "../Native/macOS.h"

namespace Cadabra { namespace Queen { namespace Core { namespace System {
	void Alert::head(StateAlert* state)
	{

	}

	void Alert::head(InputAlert* input)
	{
		NSAlert* alert = [[NSAlert alloc] init];
		[alert setMessageText: input->getTitle().toNSString()];
		[alert addButtonWithTitle: tr("Ok").toNSString()];
		if (input->getFlagSet().testFlag(InputAlert::Flag::CancelOption)) {
			[alert addButtonWithTitle: tr("Cancel").toNSString()];
		}

		NSTextField* text_field = [[NSTextField alloc] initWithFrame: NSMakeRect(0, 0, 200, 24)];
		[text_field setStringValue: input->getText().toNSString()];
		[text_field setPlaceholderString: input->getPlaceholder().toNSString()];
		[alert setAccessoryView: text_field];

		[
			alert
			beginSheetModalForWindow: QueenKeyWindow()
			completionHandler:^(NSModalResponse response)
			{
				if (response == NSAlertFirstButtonReturn) {
					[text_field validateEditing];
					input->complete(QString::fromNSString(text_field.stringValue));
				} else {
					input->cancel();
				}
				[text_field release];
			}
		];
	}

	void Alert::head(ActionAlert* action)
	{
		NSAlert* alert = [[NSAlert alloc] init];
		[alert setMessageText: action->getTitle().toNSString()];
		[alert setInformativeText: action->getText().toNSString()];

		for (AlertOption* option : action->getOptionPropertyList()) {
			if (option->getCaption().isEmpty()) {
				continue;
			}
			NSButton* button = [alert addButtonWithTitle: option->getCaption().toNSString()];
			switch (option->getFlag()) {
				case AlertOption::Flag::Discard:
				case AlertOption::Flag::Reset:
				case AlertOption::Flag::Restore:
				case AlertOption::Flag::Abort:
					[[button cell] setBackgroundColor: [NSColor redColor]];
					break;

				case AlertOption::Flag::Cancel:
					[button setContentTintColor: [NSColor redColor]];
					break;

				default:
					///
					break;
			}
		}

		[
			alert
			beginSheetModalForWindow: QueenKeyWindow()
			completionHandler:^(NSModalResponse response)
			{
				action->opt(action->getOptionPropertyList().at(response - NSAlertFirstButtonReturn));
			}
		];
	}
} } } }
