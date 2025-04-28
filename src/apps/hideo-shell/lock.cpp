module;

#include <karm-math/align.h>
#include <karm-text/loader.h>

export module Hideo.Shell:lock;

import Mdi;
import Karm.Ui;
import :model;
import :background;

namespace Hideo::Shell {

static Opt<Rc<Text::Fontface>> _blackFontface = NONE;

static Rc<Text::Fontface> blackFontface() {
    if (not _blackFontface) {
        _blackFontface = Text::loadFontfaceOrFallback("bundle://fonts-inter/fonts/Inter-Bold.ttf"_url).unwrap();
    }
    return *_blackFontface;
}

Ui::Child lock(State const& state) {
    auto [date, time] = state.dateTime;
    auto dateTime = Io::format(
        // Mon, 28 Jul
        "{}, {} {}",
        Io::toCapitalCase(date.dayOfWeek().str()),
        date.dayOfMonth() + 1,
        Io::toCapitalCase(date.month.str())
    );

    auto clock = Ui::vflow(
        0,
        Math::Align::CENTER,
        Ui::text(
            {
                .font = Text::Font{
                    blackFontface(),
                    16,
                },
            },
            dateTime
        ),
        Ui::text({
                     .font = Text::Font{
                         blackFontface(),
                         72,
                     },
                 },
                 "{02}:{02}", time.hour, time.minute)
    );

    auto hintText = Ui::vflow(
        Ui::center(Ui::icon(Mdi::CHEVRON_UP, 48)),
        Ui::center(Ui::labelLarge(state.isMobile ? "Swipe up to unlock" : "Swipe up or press any key to unlock"))
    );

    return Ui::stack(
        background(state),
        Ui::vflow(clock, Ui::grow(NONE), hintText | Ui::slideIn(Ui::SlideFrom::BOTTOM)) |
            Ui::insets(state.isMobile ? 64 : 128) |
            Ui::dragRegion() |
            Ui::dismisable(Model::bind<Unlock>(), Ui::DismisDir::TOP, 0.3) |
            Ui::align(Math::Align::VFILL | Math::Align::HCENTER)
    );
}

} // namespace Hideo::Shell
