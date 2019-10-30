import requests
from kivy.app import App
from kivy.uix.screenmanager import ScreenManager, Screen
from kivy.core.window import Window


class Setup(ScreenManager):
    pass


class ManagementScreen(Screen):

    def status(self, url):
        text = requests.get(url)
        return text.text

    def send_post(self, url):
        new_url = url[:-5] + "/now"
        status = requests.get(new_url)
        if status.text == "on":
            url = url + "?status=off"
            requests.get(url)
        else:
            url = url + "?status=on"
            requests.get(url)
            url = url[:-2] + "off"
            requests.get(url)

    def on_pre_enter(self, *args):
        Window.bind(on_keyboard=self.voltar)

    def voltar(self, window, key, *args):
        if key == 27:
            App.get_running_app().root.current = 'main'
            return True

    def on_pre_leave(self, *args):
        Window.unbind(on_keyboard=self.voltar)


class AboutScreen(Screen):

    def on_pre_enter(self, *args):
        Window.bind(on_keyboard=self.voltar)

    def voltar(self, window, key, *args):
        if key == 27:
            App.get_running_app().root.current = 'main'
            return True

    def on_pre_leave(self, *args):
        Window.unbind(on_keyboard=self.voltar)


class BusinessScreen(Screen):

    def on_pre_enter(self, *args):
        Window.bind(on_keyboard=self.voltar)

    def voltar(self, window, key, *args):
        if key == 27:
            App.get_running_app().root.current = 'main'
            return True

    def on_pre_leave(self, *args):
        Window.unbind(on_keyboard=self.voltar)


class SecurityScreen(Screen):

    def on_pre_enter(self, *args):
        Window.bind(on_keyboard=self.voltar)

    def voltar(self, window, key, *args):
        if key == 27:
            App.get_running_app().root.current = 'main'
            return True

    def on_pre_leave(self, *args):
        Window.unbind(on_keyboard=self.voltar)


class ExtraScreen(Screen):

    def on_pre_enter(self, *args):
        Window.bind(on_keyboard=self.voltar)

    def voltar(self, window, key, *args):
        if key == 27:
            App.get_running_app().root.current = 'main'
            return True

    def on_pre_leave(self, *args):
        Window.unbind(on_keyboard=self.voltar)


class CitigApp(App):

    def build(self):
        return Setup()


if __name__ == "__main__":
    CitigApp().run()
