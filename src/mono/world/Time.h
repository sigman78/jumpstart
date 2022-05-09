#pragma once

namespace mono::world {

    struct Time {
        struct Item {
            int absolute = 0;
            //
            struct Constants {
                static constexpr int days_per_month = 30;
                static constexpr int month_per_year = 12;
                static constexpr int days_per_year = days_per_month * month_per_year;
            };

            //utility:
            int day_of_year() const { return absolute % Constants::days_per_year; }
            int year() const { return absolute / Constants::days_per_year; }
            int day() const { return day_of_year() % Constants::days_per_month; }
            int month() const { return day_of_year() / Constants::month_per_year; }
        };
    };

}