#pragma once

struct Settings
{
	using ISetting = AutoTOML::ISetting;
	using bSetting = AutoTOML::bSetting;

	static void load()
	{
		try {
			const auto table = toml::parse_file("Data/SKSE/Plugins/QuickLootRE.toml"s);
			for (const auto& setting : ISetting::get_settings()) {
				setting->load(table);
			}
		} catch (const toml::parse_error& e) {
			std::ostringstream ss;
			ss
				<< "Error parsing file \'" << *e.source().path << "\':\n"
				<< '\t' << e.description() << '\n'
				<< "\t\t(" << e.source().begin << ')';
			logger::error(ss.str());
			throw std::runtime_error("failed to load settings"s);
		}
	}

	static inline bSetting closeInCombat{ "General"s, "closeInCombat"s, true };
	static inline bSetting closeOnEmpty{ "General"s, "closeOnEmpty"s, true };
	static inline bSetting dispelInvis{ "General"s, "dispelInvis"s, true };
	static inline bSetting disableAnimals{ "General"s, "disableAnimals"s, false };
	static inline bSetting disableCorpses{ "General"s, "disableCorpses"s, false };
};
