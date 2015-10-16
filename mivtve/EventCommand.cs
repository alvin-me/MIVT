using System;
using System.Reflection;
using System.Windows;
using System.Windows.Input;

namespace mivtve
{
  /// <summary>
  /// Getting WPF SizeChanged events at start-up when using MVVM and DataContext
  /// http://www.codeproject.com/Articles/210022/Getting-WPF-SizeChanged-Events-at-start-up-when-us
  /// </summary>
  public class EventCommand
  {
    public static DependencyProperty CommandProperty =
    DependencyProperty.RegisterAttached("Command",
    typeof(ICommand),
    typeof(EventCommand),
    new FrameworkPropertyMetadata(CommandChanged));

    public static void SetCommand(DependencyObject target, ICommand value)
    {
      target.SetValue(EventCommand.CommandProperty, value);
    }

    public static ICommand GetCommand(DependencyObject target)
    {
      return (ICommand)target.GetValue(CommandProperty);
    }

    private static void CommandChanged(DependencyObject target,
        DependencyPropertyChangedEventArgs e)
    {
    }

    public static DependencyProperty EventNameProperty =
       DependencyProperty.RegisterAttached("Name",
       typeof(string),
       typeof(EventCommand),
       new FrameworkPropertyMetadata(NameChanged));

    public static void SetName(DependencyObject target, string value)
    {
      target.SetValue(EventCommand.EventNameProperty, value);
    }

    public static string GetName(DependencyObject target)
    {
      return (string)target.GetValue(EventNameProperty);
    }

    private static void NameChanged(DependencyObject target, DependencyPropertyChangedEventArgs e)
    {
      UIElement element = target as UIElement;

      if (element != null)
      {
        // If we're putting in a new command and there wasn't one already hook the event
        if ((e.NewValue != null) && (e.OldValue == null))
        {
          EventInfo eventInfo = element.GetType().GetEvent((string)e.NewValue);

          Delegate d = Delegate.CreateDelegate(eventInfo.EventHandlerType,
            typeof(EventCommand).GetMethod("Handler",
            BindingFlags.NonPublic | BindingFlags.Static));

          eventInfo.AddEventHandler(element, d);
        }
        // If we're clearing the command and it wasn't already null unhook the event
        else if ((e.NewValue == null) && (e.OldValue != null))
        {
          EventInfo eventInfo = element.GetType().GetEvent((string)e.OldValue);

          Delegate d = Delegate.CreateDelegate(eventInfo.EventHandlerType,
                       typeof(EventCommand).GetMethod("Handler"));

          eventInfo.RemoveEventHandler(element, d);
        }
      }
    }

    static void Handler(object sender, EventArgs e)
    {
      UIElement element = (UIElement)sender;
      ICommand command = (ICommand)element.GetValue(EventCommand.CommandProperty);

      var src = Tuple.Create(sender, e);

      if (command != null && command.CanExecute(src) == true)
        command.Execute(src);
    }
  }
}
