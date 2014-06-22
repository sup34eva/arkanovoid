# Recupère les item de l'utilisateur
getPerson window.user, (xml) ->
    console.log xml.root
    for i of xml.root
        item = xml.root[i]
        $("[data-item='#{item.item_id}']").parent().parent().attr 'data-amount', item.quantity

# Ajoute un item
$('form').submit (e) ->
    e.preventDefault()
    itemid = Number($(this).attr('data-item'))
    alltime = Number($(this).find('input[type="checkbox"]').prop('checked'))
    form = $(this)
    if not Number.isNaN alltime
        itemid += alltime
    for i in [0...$(this).find('input[type="number"]').val()]
        addItemForPerson window.user, itemid, (code) ->
            form.parent().parent().attr('data-amount', Number(form.parent().parent().attr('data-amount')) + 1) unless code isnt 201
