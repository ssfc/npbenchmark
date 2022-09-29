////////////////////////////////
/// usage : 1.	utility for SVG graph generation.
/// 
/// note  : 1.	
////////////////////////////////

#ifndef CN_HUST_GOAL_SYSTEM_SVG_DRAWER_H
#define CN_HUST_GOAL_SYSTEM_SVG_DRAWER_H


#include <algorithm>
#include <string>
#include <random>
#include <iostream>
#include <sstream>
#include <vector>
#include <array>


namespace goal {

struct RandColor {
	static constexpr auto ColorCodeChar = "0123456789ABCDEF";
	static constexpr int ColorCodeBase = 16;
	static constexpr int ColorCodeLen = 6;

	void next() {
		for (int i = 0; i < ColorCodeLen; ++i) {
			int c = r() % ColorCodeBase;
			bcolor[i] = ColorCodeChar[c];
			tcolor[i] = ColorCodeChar[(c > (ColorCodeBase / 2)) ? 0 : (ColorCodeBase - 1)]; // (c + ColorCodeBase / 2) % ColorCodeBase
			scolor[i] = tcolor[i];
		}
	}

	char tcolor[ColorCodeLen + 2] = { '#' }; // text color.
	char bcolor[ColorCodeLen + 2] = { '#' }; // background color.
	char scolor[ColorCodeLen + 2] = { '#' }; // stroke color.
	std::mt19937 r = std::mt19937(std::random_device()());
};

template<typename Coord = double>
struct SvgDrawer {
	std::string toStr(Coord viewBoxMinX, Coord viewBoxMinY, Coord viewBoxWidth, Coord viewBoxHeight,
		Coord graphWidth = 100, Coord graphHeight = 100, std::string widthUnit = "%", std::string heightUnit = "%",
		std::string style = "<style>rect { fill:grey; stroke:black; stroke-width: 1px; } line { stroke:black; stroke-width: 1px; } path { stroke: black; fill:transparent; }</style>") const {
		std::ostringstream out;
		out << "<!DOCTYPE html>\n"
			<< "<html>\n"
			<< "\t<head>\n"
			<< "\t\t<meta charset='utf-8'>\n"
			<< "\t\t<title>Visualization</title>\n"
			<< "\t\t" << style << "\n"
			<< "\t</head>\n"
			<< "\t<body>\n" // style='text-align:center;'
			<< "\t\t<svg width='" << graphWidth << widthUnit << "' height='" << graphHeight << heightUnit << "' viewBox='" << viewBoxMinX << " " << viewBoxMinY << " " << viewBoxWidth << " " << viewBoxHeight << "'>\n";
		out << oss.str();
		out << "\t\t</svg>\n"
			<< "\t</body>\n"
			<< "</html>\n";
		return out.str();
	}

	void pathBegin() { oss << "<path d='"; }
	void pathEnd() { oss << "' />\n"; }
	void pathLine(Coord x1, Coord y1, Coord x2, Coord y2) {
		oss << "M" << x1 << " " << y1;
		if (x1 == x2) {
			oss << " v" << (y2 - y1);
		} else if (y1 == y2) {
			oss << " h" << (x2 - x1);
		} else {
			oss << " L" << x2 << " " << y2;
		}
		oss << "\n";
	}
	void pathPoly(const std::vector<std::array<Coord, 2>>& coords) {
		pathBegin();
		auto s = coords.begin();
		oss << "M" << (*s)[0] << " " << (*s)[1];
		for (auto d = coords.begin() + 1; d != coords.end(); s = d, ++d) {
			if ((*s)[0] == (*d)[0]) {
				Coord dy = (*d)[1] - (*s)[1];
				oss << " v" << dy;
			} else if ((*s)[1] == (*d)[1]) {
				Coord dx = (*d)[0] - (*s)[0];
				oss << " h" << dx;
			} else {
				oss << " L" << (*d)[0] << " " << (*d)[1];
			} // OPT[szx][9]: merge parallel segments.
		}
		pathEnd();
	}
	void pathRect(Coord x, Coord y, Coord w, Coord h) {
		oss << "M" << x << " " << y << " h" << w << " v" << h
			<< " h" << -w << " Z\n";
	}

	void rect(Coord x, Coord y, Coord w, Coord h) {
		oss << "<rect x='" << x << "' y='" << y << "' width='" << w << "' height='" << h << "' />\n";
	}
	void rect(Coord x, Coord y, Coord w, Coord h, const std::string& bcolor, const std::string& scolor = "black") {
		oss << "<rect x='" << x << "' y='" << y << "' width='" << w << "' height='" << h << "' style='fill:" << bcolor << "; stroke:" << scolor << "; stroke-width:1' />\n";
	}
	void rect(Coord x, Coord y, Coord w, Coord h, const std::string& bcolor, const std::string& scolor, const std::string& tcolor, const std::string& label) {
		rect(x, y, w, h, bcolor, scolor);
		oss << "<text x='" << x + w / 3 << "' y='" << y + h / 3 << "' text-anchor='middle' alignment-baseline='middle' style='fill:" << tcolor << ";font-size:32px'>" << label << "</text>\n";
	}
	void rectRC(Coord x, Coord y, Coord w, Coord h) {
		rc.next();
		rect(x, y, w, h, rc.bcolor, rc.scolor);
	}
	void rectRC(Coord x, Coord y, Coord w, Coord h, const std::string& label) {
		rc.next();
		rect(x, y, w, h, rc.bcolor, rc.scolor, rc.tcolor, label);
	}

	void line(Coord x1, Coord y1, Coord x2, Coord y2) {
		oss << "<line x1='" << x1 << "' y1='" << y1 << "' x2='" << x2 << "' y2='" << y2 << "' />\n";
	}
	void line(Coord x1, Coord y1, Coord x2, Coord y2, int width, std::string scolor = "black", std::string style = "") { // "stroke-dasharray:12 4".
		oss << "<line x1='" << x1 << "' y1='" << y1 << "' x2='" << x2 << "' y2='" << y2 << "' style='stroke:" << scolor << "; stroke-width:" << width << "; " << style << "' />\n";
	}

	void circle(Coord x, Coord y, Coord r) {
		oss << "<circle cx='" << x << "' cy='" << y << "' r='" << r << "' />\n";
	}
	void circle(Coord x, Coord y, Coord r, const std::string& label) {
		oss << "<circle cx='" << x << "' cy='" << y << "' r='" << r << "' />\n";
		oss << "<text x='" << x << "' y='" << y << "' text-anchor='middle' alignment-baseline='middle' font-size='" << r << "'>" << label << "</text>\n";
	}
	void circle(Coord x, Coord y, Coord r, const std::string& bcolor, const std::string& scolor) {
		oss << "<circle cx='" << x << "' cy='" << y << "' r='" << r << "' style='fill:" << bcolor << "; stroke:" << scolor << "; stroke-width:1' />\n";
	}

	std::ostringstream oss;
	RandColor rc;
};

template<typename Coord = double>
struct CanvasDrawer {
	std::string toStr(Coord viewBoxMinX, Coord viewBoxMinY, Coord viewBoxWidth, Coord viewBoxHeight,
		Coord graphWidth = 100, Coord graphHeight = 100, std::string widthUnit = "%", std::string heightUnit = "%",
		std::string style = "") const {
		std::ostringstream out;
		out << "<!DOCTYPE html>\n"
			<< "<html>\n"
			<< "\t<head>\n"
			<< "\t\t<meta charset='utf-8'>\n"
			<< "\t\t<title>Visualization</title>\n"
			<< "\t\t<script type='application/javascript'>\n"
			<< "\t\t\tfunction draw() {\n"
			<< "\t\t\t\tvar canvas = document.getElementById('szx');\n"
			<< "\t\t\t\tif (!canvas.getContext) { return; }\n"
			<< "\t\t\t\tvar ctx = canvas.getContext('2d');\n"
			<< "\t\t\t\tctx.beginPath();\n"
			<< "\t\t\t\tfunction l(x1, y1, x2, y2) { ctx.moveTo(x1, y1); ctx.lineTo(x2, y2); }\n"
			<< "\t\t\t\tfunction r(x, y, w, h) { ctx.rect(x, y, w, h) }\n"
			<< "\t\t\t\tfunction fr(x, y, w, h) { ctx.fillRect(x, y, w, h) }\n";
		//<< "\t\t\t\tctx.fillStyle = 'white';"
		//<< "\t\t\t\tctx.strokeStyle = 'black';"
		//<< "\t\t\t\tctx.lineWidth = 1;";
		out << oss.str();
		out << "\t\t\t\tctx.stroke();\n"
			<< "\t\t\t}\n"
			<< "\t\t</script>\n"
			<< "\t</head>\n"
			<< "\t<body onload='draw();'>\n" // style='text-align:center;'
			<< "\t\t<canvas id='szx' width='" << (graphWidth * scale) << widthUnit << "' height='" << (graphHeight * scale) << heightUnit << "'></canvas>\n"
			<< "\t</body>\n"
			<< "</html>\n";
		return out.str();
	}

	void rect(Coord x, Coord y, Coord w, Coord h) {
		oss << "r(" << (x * scale) << "," << (y * scale) << "," << (w * scale) << "," << (h * scale) << ");\n";
	}
	void line(Coord x1, Coord y1, Coord x2, Coord y2) {
		oss << "l(" << (x1 * scale) << "," << (y1 * scale) << "," << (x2 * scale) << "," << (y2 * scale) << ");\n";
	}

	std::ostringstream oss;
	Coord scale = 2;
};

}


#endif // CN_HUST_GOAL_SYSTEM_SVG_DRAWER_H
