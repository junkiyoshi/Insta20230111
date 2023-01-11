#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 100, true, true, true);
	this->word = "water";
}
//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();

	int radius_start = 10 - ofGetFrameNum() % 10;
	for (int radius = radius_start; radius <= 500; radius += 10) {

		for (int deg = 0; deg < 360; deg += 1) {

			auto z = radius > 200 ? 0 : ofMap(radius, 0, 200, -1 * pow(200 - radius, 1.1), 0);
			
			int next_radius = radius + 10;
			int next_z = next_radius > 200 ? 0 : ofMap(next_radius, 0, 200, -1 * pow(200 - next_radius, 1.1), 0);
			vector<glm::vec3> vertices;
			vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), z));
			vertices.push_back(glm::vec3(radius * cos((deg + 1) * DEG_TO_RAD), radius * sin((deg + 1) * DEG_TO_RAD), z));
			vertices.push_back(glm::vec3(next_radius * cos((deg + 1) * DEG_TO_RAD), next_radius * sin((deg + 1) * DEG_TO_RAD), next_z));
			vertices.push_back(glm::vec3(next_radius * cos(deg * DEG_TO_RAD), next_radius * sin(deg * DEG_TO_RAD), next_z));

			face.addVertices(vertices);

			face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 2); face.addIndex(face.getNumVertices() - 3);
			face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 3); face.addIndex(face.getNumVertices() - 4);

		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(-55);

	ofSetColor(0);
	this->face.draw();

	for (int i = 0; i < 500; i++) {

		auto radius = (int)(ofRandom(500) + ofGetFrameNum() * ofRandom(4, 10)) % 500;
		auto deg = ofRandom(360);
		auto size = ofRandom(10, 30);

		auto rotation_deg = ofRandom(360);
		auto rotation_z = glm::rotate(glm::mat4(), rotation_deg * (float)DEG_TO_RAD, glm::vec3(0, 0, 1));

		ofPushMatrix();
		ofTranslate(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), 0);

		ofPath chara_path = this->font.getCharacterAsPoints(this->word[ofRandom(this->word.size())], true, false);
		vector<ofPolyline> outline = chara_path.getOutline();

		ofFill();
		ofSetColor(0);
		ofBeginShape();
		for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

			ofNextContour(true);

			auto vertices = outline[outline_index].getVertices();
			for (auto& vertex : vertices) {

				glm::vec3 location = vertex / 100 * size;
				location -= glm::vec3(size * 0.5, -size * 0.5, 0);

				location = glm::vec4(location, 0) * rotation_z;

				int len = glm::length(glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD)) + location);
				location.z = len > 200 ? 0 : ofMap(len, 0, 200, -1 * pow(200 - len, 1.1), 0);

				ofVertex(location + glm::vec3(0, 0, 2));
			}
		}
		ofEndShape(true);

		ofNoFill();
		ofSetColor(255);
		ofBeginShape();
		for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

			ofNextContour(true);

			auto vertices = outline[outline_index].getVertices();
			for (auto& vertex : vertices) {

				glm::vec3 location = vertex / 100 * size;
				location -= glm::vec3(size * 0.5, -size * 0.5, 0);

				location = glm::vec4(location, 0) * rotation_z;

				int len = glm::length(glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD)) + location);
				location.z = len > 200 ? 0 : ofMap(len, 0, 200, -1 * pow(200 - len, 1.1), 0);

				ofVertex(location + glm::vec3(0, 0, 2));
			}
		}
		ofEndShape(true);


		ofPopMatrix();
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}